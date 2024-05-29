#include "events.h"
#include "math.h"
#include "utilities.h"

Event getNextBallBallCollision(Table *table) {

  Event event = {INFINITY, INVALID, 0, 0, 0};

  size_t colisionNumber = 0;
  // TODO: Only save space for balls moving

  size_t *balls1 = (size_t *) malloc(sizeof(size_t) * (table->ballNumber + 1) * table->ballNumber / 2);
  size_t *balls2 = (size_t *) malloc(sizeof(size_t) * (table->ballNumber + 1) * table->ballNumber / 2);
  QuarticCoeff *coeficients = (QuarticCoeff *) malloc(sizeof(QuarticCoeff) * (table->ballNumber + 1) * table->ballNumber / 2);

  for (size_t i = 0; i < table->ballNumber - 1; i++) {

    Ball *ball1 = table->balls[i];

    for (size_t j = i; j < table->ballNumber; j++) {

      Ball *ball2 = table->balls[j];

      if (ball1->state == POCKETED || ball2->state == POCKETED)
        continue;
      if (ballNotMoving(ball1) && ballNotMoving(ball2))
        continue;

      vector_t positionDiff = {ball1->position.x - ball2->position.x, ball1->position.y - ball2->position.y};
      if (magnitudeOf(positionDiff) < (ball1->radius + ball2->radius))
        continue;
      balls2[colisionNumber] = j;

      coeficients[colisionNumber] = getBallBallCollisionCoeff(ball1, ball2, table->rollingFriction, table->slidingFriction, table->gravityAcceleration);
      colisionNumber++;
    }
  }
  if (colisionNumber == 0)
    return event;
  colisionNumber--;

  double collisionTime;
  size_t i = findSmallerCoeficient(colisionNumber, coeficients, &collisionTime);
  if (i == -1)
    return event;

  event.type = BALL_BALL;
  event.ball1 = balls1[i];
  event.ball2 = balls2[i];
  event.time = collisionTime;

  free(coeficients);
  return event;
}
