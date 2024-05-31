
#include "table.h"
#include "../labs/graphics.h"
#include "../physics/utilities.h"
#include "../viewer/cueViewer.h"
#include "../xpms/table.xpm"
#include "../xpms/biggerTable.xpm"
#include "math.h"

Table *newTable() {
  Table *table = (Table *) malloc(sizeof(Table));

  // Set balls
  table->ballNumber = 7;
  table->balls = (Ball **) malloc(sizeof(Ball *) * table->ballNumber);

  vector_t cueBallPosition = {500, 500};
  table->balls[0] = newBall(cueBallPosition);
  vector_t otherBallPosition = {100, 100};
  table->balls[1] = newBall(otherBallPosition);
  otherBallPosition.x = 100;
  otherBallPosition.y = 500;
  table->balls[2] = newBall(otherBallPosition);
  otherBallPosition.x = 500;
  otherBallPosition.y = 100;
  table->balls[3] = newBall(otherBallPosition);
  otherBallPosition.x = 300;
  otherBallPosition.y = 400;
  table->balls[4] = newBall(otherBallPosition);
  otherBallPosition.x = 700;
  otherBallPosition.y = 400;
  table->balls[5] = newBall(otherBallPosition);
  otherBallPosition.x = 100;
  otherBallPosition.y = 800;
  table->balls[6] = newBall(otherBallPosition);
  // Set image
  xpm_image_t img;
  xpm_load(biggerTableXpm, XPM_8_8_8, &img);
  table->img = img;

  // Set pockets
  table->pocketRadius = 25;

  // Set cushions

  // Set mouse
  table->mouse = newMouse();

  // Set cue
  table->cue = newCue();
  updateCueState(table, false);
  table->maxSpeedShot = 10;

  // Set state
  table->state = AIMING;


  // Set physics attributes
  table->gravityAcceleration = 9.81;
  table->slidingFriction = 0.2;
  table->spinningFriction = 0.01;
  table->rollingFriction = 0.01;
  table->cushionRestitution = 0.85;

  return table;
}

void destroyTable(Table *table) {
  for (size_t i = 0; i < table->ballNumber; i++) {
    free(table->balls[i]);
  }
  free(table->mouse);
  free(table->cue);
}

int drawTable(Table *table) {


  drawXPMImage(table->img, 512, 384, 0);


  for (size_t i = 0; i < table->ballNumber; i++) {
    Ball *ball = table->balls[i];
    if (drawXPMImage(getBallImage(ball), getBallPosition(ball).x, getBallPosition(ball).y, 0))
      return 1;
  }

  switch (table->state) {
    case AIMING:
    case SHOOTING:
      drawCue(table);
      break;
    default:
      break;
  }
  if (vg_draw_rectangle(table->mouse->pos.x, table->mouse->pos.y, 9, 9, 0xff0000))
    return 1;
  return 0;
}

bool getColisionPoint(Table *table, vector_t *colisionPoint) {

  Cue *cue = table->cue;
  Ball *cueBall = table->balls[0];
  Ball *collisionBall = NULL;
  uint16_t collisionXDistance = UINT16_MAX;

  for (size_t i = 1; i < table->ballNumber; i++) {
    Ball *ball = table->balls[i];

    vector_t s = {cueBall->position.x - ball->position.x, cueBall->position.x - ball->position.y};
    double b = s.x * cue->directionVector.x + s.y * cue->directionVector.y;
    // TODO Fix ball radius hard code
    double c = s.x * s.x + s.y * s.y - 40 * 40;
    double h = b * b - c;
    if (h < 0) {
      continue;
    }
    else {
      h = sqrt(h);
      double t = 0 - b - h;
      if (t < 0)
        continue;
      else {
        printf("Found a collision\n");
        uint16_t distance = abs(ball->position.x - cueBall->position.x);
        if (distance < collisionXDistance) {
          collisionXDistance = distance;
          collisionBall = ball;

          colisionPoint->x = cueBall->position.x + cue->directionVector.x * t;
          colisionPoint->y = cueBall->position.y + cue->directionVector.y * t;

          cue->targetBallCenter = ball->position;
        }
      }
    }
  }
  if (!collisionBall) {
    // TODO Colision with the sides
    colisionPoint->x = cueBall->position.x + cue->directionVector.x * 500;
    colisionPoint->y = cueBall->position.y + cue->directionVector.y * 500;
    return false;
  }
  return true;
}

int updateCueState(Table *table, bool power) {

  vector_t target;
  vector_t cueBall = table->balls[0]->position;
  Mouse *mouse = table->mouse;
  Cue *cue = table->cue;
  if (power) {

    target = table->mouse->savedPos;
    // Calculate the dot product
    cue->charge += (cue->directionVector.x * -mouse->delta.x + cue->directionVector.y * -mouse->delta.y) / cue->maximumDistance * cue->powerSensitivity;

    cue->charge = MAX(0, cue->charge);
    cue->charge = MIN(1, cue->charge);
  }
  else {
    target = table->mouse->pos;
    cue->directionVector = unitVector(cueBall, target);
    cue->angle = atan2(cueBall.y - target.y, cueBall.x - target.x);

    if (getColisionPoint(table, &cue->colisionPoint)) {

      // Vector that connects target ball and colision point
      cue->targetBallVec = unitVector(cue->colisionPoint, cue->targetBallCenter);
      cue->guideScalar = dotProduct(cue->directionVector, cue->targetBallVec);

      double sinOfImpact = cue->directionVector.x *cue->targetBallVec.y  - cue->directionVector.y * cue->targetBallVec.x;
     
      if (sinOfImpact < 0) {

        cue->whiteBallVec.x = -cue->targetBallVec.y;
        cue->whiteBallVec.y = cue->targetBallVec.x;
        printf("Angle greater than 0\n");
      }
      else {
        cue->whiteBallVec.x = cue->targetBallVec.y;
        cue->whiteBallVec.y = -cue->targetBallVec.x;
        printf("Angle less than 0\n");
      }

      cue->colides = true;
    }
    else {
      cue->targetBallVec.x = 0;
      cue->targetBallVec.y = 0;
      cue->whiteBallVec.x = 0;
      cue->whiteBallVec.y = 0;

      cue->colides = false;
    }
  }

  float distance = cue->charge * cue->maximumDistance + cue->minimalDistance;
  cue->position.x = cos(cue->angle) * (212 + distance) + cueBall.x;
  cue->position.y = sin(cue->angle) * (212 + distance) + cueBall.y;

  return 0;
}
