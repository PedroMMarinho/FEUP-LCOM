#include "evolve.h"
#include "physicsMacros.h"
#include "utilities.h"
#include <math.h>

void evolveBallMotion(Table *table, Ball *ball, double time) {

  switch (getBallState(ball)) {
    case STATIONARY:
    case POCKETED:
      printf("Nothing changes in the ball\n");
      return;
      break;
    case SLIDING: {

      double slideTime = getSlideTime(ball, table->slidingFriction, table->gravityAcceleration);
      char a[20];
      sprintf(a, "%f", slideTime);
      printf("The time is: %s\n", a);

      // if t >= dtau_E_slide:
      //     rvw = evolve_slide_state(rvw, R, m, u_s, u_sp, g, dtau_E_slide)
      //     state = const.rolling
      //     t -= dtau_E_slide
      // else:
      //     return evolve_slide_state(rvw, R, m, u_s, u_sp, g, t), const.sliding

      evolveSlideState(ball, MIN(slideTime, time), table->spinningFriction, table->slidingFriction, table->gravityAcceleration);

      break;
    }

    case ROLLING:

      break;

    case SPINNING:

      break;
    default:
      break;
  }
}

void evolveSlideState(Ball *ball, double t, double uSpinning, double uSliding, double g) {

  // For printing floats
  char a[40];

  if (!t)
    return;

  // Calculate the angle for coordinate transformation
  double ang = angle(ball->velocity);

  // debugger
  printf("--- INITAL STATE ---");
  sprintf(a, "%fl", ang);
  printf("\n\nthe angle: %s\n", a);
  sprintf(a, "%fl", ang * 180 / M_PI);
  printf("in degrees: %s\n", a);

  // Change coordinates (aka. rotate) each component
  vector_t velocityB = rotate2d(ball->velocity, -ang);

  vector_t xyAngVelocityB = {ball->ang_velocity.x, ball->ang_velocity.y};
  xyAngVelocityB = rotate2d(xyAngVelocityB, -ang);

  vector_t relVelocityB = rotate2d(normalizeVector(relativeVelocity(ball)), -ang);

  // debugger
  printf("\n\n---- Rotated ----\n");
  sprintf(a, "%fl", velocityB.x);
  printf("The velocity: x.%s  ", a);
  sprintf(a, "%fl", velocityB.y);
  printf("y. %s\n", a);

  sprintf(a, "%fl", xyAngVelocityB.x);
  printf("The angularVelocity: x.%s  ", a);
  sprintf(a, "%fl", xyAngVelocityB.y);
  printf("y. %s  ", a);
  sprintf(a, "%fl", ball->ang_velocity.z);
  printf("z. %s\n", a);

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

  // debugger
  printf("\n\n -------- CHANGED -----\n");

  sprintf(a, "%fl", positionB.x);
  printf("The position: x.%s  ", a);
  sprintf(a, "%fl", positionB.y);
  printf("y. %s\n", a);

  sprintf(a, "%fl", velocityB.x);
  printf("The velocity: x.%s  ", a);
  sprintf(a, "%fl", velocityB.y);
  printf("y. %s\n", a);

  sprintf(a, "%fl", xyAngVelocityB.x);
  printf("The angularVelocity: x.%s  ", a);
  sprintf(a, "%fl", xyAngVelocityB.y);
  printf("y. %s  ", a);
  sprintf(a, "%fl", ball->ang_velocity.z);
  printf("z. %s\n", a);

  // Return coords to table coordinates

  vector_t positionT = rotate2d(positionB, ang);
  ball->position.x = ball->position.x + positionT.x;
  ball->position.y = ball->position.y + positionT.y;

  ball->velocity = rotate2d(velocityB, ang);

  vector_t xyAngVelocityT = rotate2d(xyAngVelocityB, ang);
  ball->ang_velocity.x = xyAngVelocityT.x;
  ball->ang_velocity.y = xyAngVelocityT.y;

  // debugger
  printf("\n\n---- FINAL COORDINATES -----\n");

  sprintf(a, "%fl", ball->position.x);
  printf("The position: x.%s  ", a);
  sprintf(a, "%fl", ball->position.y);
  printf("y. %s\n", a);

  sprintf(a, "%fl", ball->velocity.x);
  printf("The velocity: x.%s  ", a);
  sprintf(a, "%fl", ball->velocity.y);
  printf("y. %s\n", a);

  sprintf(a, "%fl", ball->ang_velocity.x);
  printf("The angVelocity: x.%s  ", a);
  sprintf(a, "%fl", ball->ang_velocity.y);
  printf("y. %s ", a);
  sprintf(a, "%fl", ball->ang_velocity.z);
  printf("z. %s \n\n", a);
}

void evolvePrependicularSpin(Ball *ball, double t, double uSpinning, double g) {

  // If angVelocity is really small or time is 0
  if (abs(ball->ang_velocity.z) < EPS || !t) {
    return;
  }

  double alpha = 5 * uSpinning * g / (2 * ball->radius);
  // Limit t so that decay stops at 0
  t = MIN(t, ball->ang_velocity.z / alpha);
  int sign = ball->ang_velocity.z > 0 ? 1 : -1;
  ball->ang_velocity.z = ball->ang_velocity.z - sign * alpha * t;
}
