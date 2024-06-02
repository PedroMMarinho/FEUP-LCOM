#include "evolve.h"
#include "physicsMacros.h"
#include "utilities.h"
#include "resolve/resolver.h"
#include <math.h>


void solveBallCollision(Ball* ball1, Ball* ball2){

  if (ballNotMoving(ball1) && ballNotMoving(ball2)) return;

  vector_t connection = {ball1->position.x - ball2->position.x, ball1->position.y - ball2->position.y};
  double distance = magnitudeOf(connection);
  connection = normalizeVector(connection);

  if (distance <= ball1->radius + ball2->radius){
    printf("COLLISION\n");
    resolveBallBall(ball1, ball2);
  }
}

void solvePocket(Table* table, Ball* ball, Pocket* pocket){

  if (ballNotMoving(ball)) return;
    
  vector_t connection = {ball->position.x - pocket->position.x, ball->position.y - pocket->position.y};
  double distance = magnitudeOf(connection);
  if (distance <= pocket->radius){ // TODO2 UPDATE VALUES IN THE TABLE
    printf("POCKET\n");
    ball->state = POCKETED;
  }
}


bool solveLinearCushion(Table* table, Ball* ball, LinearCushion* cushion){

  if (ballNotMoving(ball)) return false;


  vector_t closerPoint = linePointClosestTo(cushion->p1, cushion->p2, ball->position);
  

  if (closerPoint.x > cushion->p1.x && closerPoint.x > cushion->p2.x) return false;
  if (closerPoint.x < cushion->p1.x && closerPoint.x < cushion->p2.x) return false;
  if (closerPoint.y > cushion->p1.y && closerPoint.y > cushion->p2.y) return false;
  if (closerPoint.y < cushion->p1.y && closerPoint.y < cushion->p2.y) return false;


  vector_t normal = cushion->normal;

  vector_t connection = {closerPoint.x-(ball->position.x-ball->radius*normal.x), closerPoint.y-(ball->position.y - ball->radius * normal.y)};
  if (dotProduct(normal, connection) > 0){
    ball->position.x = closerPoint.x + (ball->radius + EPS_SPACE) * normal.x;
    ball->position.y = closerPoint.y + (ball->radius + EPS_SPACE) * normal.y;

    resolveBallCushion(ball, normal, table->cushionRestitution);
    return true;
  }
  return false;
}

void solveCircularCushion(Table* table, Ball* ball, CircularCushion* cushion){


  vector_t connection = {ball->position.x - cushion->position.x, ball->position.y - cushion->position.y};
  double distance = magnitudeOf(connection);
  connection = normalizeVector(connection);


  // TODO - Fix the use of EPS to make it regular
  double error = ball->radius + cushion->radius + EPS_SPACE - distance;
  if (error > 0) {
    printf("Colling\n");
    ball->position.x += error * connection.x;
    ball->position.y += error * connection.y;

    resolveBallCushionRealistic(ball, CircularCushionNormal(cushion, ball), table->cushionRestitution, table->cushionFriction);
    
    
  }
}


void evolveBallMotion(Table *table, Ball *ball, double time) {
  if (ball->transition != NULL) {
    ball->transition->time -= time;
  }

  switch (getBallState(ball)) {
    case STATIONARY:
    case POCKETED:
      return;
      break;
    case SLIDING: {
      evolveSlideState(ball, time, table->spinningFriction, table->slidingFriction, table->gravityAcceleration);
      // See if ball no longer rolling
      vector_t relVelocity = relativeVelocity(ball);
      if (magnitudeOf(relVelocity) < MINIMAL_SPEED) {
        ball->state = ROLLING;
      }
      break;
    }

    case ROLLING: {

      evolveRollState(ball, time, table->rollingFriction, table->spinningFriction, table->gravityAcceleration);

      if (abs(ball->velocity.x) < MINIMAL_SPEED && abs(ball->velocity.y) < MINIMAL_SPEED) {
        ball->velocity.x = 0;
        ball->velocity.y = 0;
        ball->state = SPINNING;
      }

      if (abs(ball->ang_velocity.z) < MINIMAL_SPEED) {
        ball->state = STATIONARY;
      }

      break;
    }

    case SPINNING: {

      evolvePrependicularSpin(ball, time, table->spinningFriction, table->gravityAcceleration);

      if (abs(ball->ang_velocity.z) < MINIMAL_SPEED) {
        ball->state = STATIONARY;
      }

      break;
    }
    default:
      break;
  }
}

void evolveRollState(Ball *ball, double t, double uRolling, double uSpinning, double g) {

  // For printing floats

  if (!t)
    return;

  vector_t v0 = normalizeVector(ball->velocity);

  ball->position.x = ball->position.x + ball->velocity.x * t - 0.5 * uRolling * g * t * t * v0.x;
  ball->position.y = ball->position.y + ball->velocity.y * t - 0.5 * uRolling * g * t * t * v0.y;

  ball->velocity.x = ball->velocity.x - uRolling * g * t * v0.x;
  ball->velocity.y = ball->velocity.y - uRolling * g * t * v0.y;

  vector_t temp = {ball->velocity.x / ball->radius, ball->velocity.y / ball->radius};
  vector_t xyAngVelocity = rotate2d(temp, M_PI / 2);
  ball->ang_velocity.x = xyAngVelocity.x;
  ball->ang_velocity.y = xyAngVelocity.y;
  evolvePrependicularSpin(ball, t, uSpinning, g); 
}

void evolveSlideState(Ball *ball, double t, double uSpinning, double uSliding, double g) {
  vector_t vinit = ball->velocity;
  if (!t)
    return;

  // Calculate the angle for coordinate transformation
  double ang = angle(ball->velocity);

  // Change coordinates (aka. rotate) each component
  vector_t velocityB = rotate2d(ball->velocity, -ang);

  vector_t xyAngVelocityB = {ball->ang_velocity.x, ball->ang_velocity.y};
  xyAngVelocityB = rotate2d(xyAngVelocityB, -ang);

  vector_t relVelocityB = rotate2d(normalizeVector(relativeVelocity(ball)), -ang);

  // Use the formulas
  vector_t positionB;
  positionB.x = velocityB.x * t - 0.5 * uSliding * g * t * t * relVelocityB.x;
  positionB.y = -0.5 * uSliding * g * t * t * relVelocityB.y;

  velocityB.x = velocityB.x - uSliding * g * t * relVelocityB.x;
  velocityB.y = velocityB.y - uSliding * g * t * relVelocityB.y;

  double denominator = 1.0 / (2.0 * ball->radius);
  xyAngVelocityB.x = xyAngVelocityB.x - 5 * denominator * uSliding * g * t * relVelocityB.y;
  xyAngVelocityB.y = xyAngVelocityB.y - 5 * denominator * uSliding * g * t * -relVelocityB.x;

  evolvePrependicularSpin(ball, uSpinning, g, t);

  // Return coords to table coordinates

  vector_t positionT = rotate2d(positionB, ang);
  ball->position.x = ball->position.x + positionT.x;
  ball->position.y = ball->position.y + positionT.y;

  ball->velocity = rotate2d(velocityB, ang);

  vector_t xyAngVelocityT = rotate2d(xyAngVelocityB, ang);
  ball->ang_velocity.x = xyAngVelocityT.x;
  ball->ang_velocity.y = xyAngVelocityT.y;
  vector_t vfin = ball->velocity ;
  vfin.x -= vinit.x;
  vfin.y -= vinit.y;
  printf("Velocity diff:\n");
  printVector(vfin);
  
}

void evolvePrependicularSpin(Ball *ball, double t, double uSpinning, double g) {

  // If angVelocity is really small or time is 0
  if (abs(ball->ang_velocity.z) < EPS || !t) {
    return;
  }

  double alpha = 5.0 * uSpinning * g / (2.0 * ball->radius);
  // Limit t so that decay stops at 0
  t = MIN(t, abs(ball->ang_velocity.z) / alpha);
  int sign = ball->ang_velocity.z > 0 ? 1 : -1;
  ball->ang_velocity.z -= sign * alpha * t;
}
