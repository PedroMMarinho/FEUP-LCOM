#include "resolver.h"
#include "../events.h"
#include "../physicsMacros.h"
#include "../utilities.h"
#include <math.h>
// TODO: REMOVE ASSERT, FOR TESTING ONLY
#include <assert.h>

void resolveEvent(Table *table, Event event) {
  printf("Resolving eventss\n");
  switch (event.type) {
    case INVALID:
      return;
      break;
      // Assert that velocity is almost 0 and that set them to 0
    case SPINNING_STATIONARY:
    case ROLLING_STATIONARY:
      printf("Stationary endds\n");
      assertStationary(event.ball1);
      updateBallNextTransition(table, event.ball1);
      break;
    case ROLLING_SPINNING:
      assertSpinning(event.ball1);
      updateBallNextTransition(table, event.ball1);
      break;
    case SLIDING_ROLLING:
      printf("SOLVING SLIDING_ROLLING PROBLOEM\n");
      event.ball1->state = ROLLING;
      updateBallNextTransition(table, event.ball1);
      break;
    case BALL_BALL:
      resolveBallBall(event.ball1, event.ball2);
      updateBallNextTransition(table, event.ball1);
      updateBallNextTransition(table, event.ball2);
      break;
    case BALL_CUSHION:
      resolveBallCushion(event.ball1, event.cushion, table->cushionRestitution);
      updateBallNextTransition(table, event.ball1);
      break;
    case BALL_POCKET:
      resolveBallPocket(event.ball1, table->pockets[event.pocket]);
      updateBallNextTransition(table, event.ball1);
      break;
    case STICK_BALL:
      // CHECK: WHITE IS ALWAYS BALL 0 ??
      resolveStickBall(table->cue, event.ball1, table->maxSpeedShot);
      updateBallNextTransition(table, event.ball1);
      break;
    default:
      break;
  }
}

void resolveBallBall(Ball *ball1, Ball *ball2) {
  makeBallsKiss(ball1, ball2);


  vector_t posVec = {ball2->position.x - ball1->position.x, ball2->position.y - ball1->position.y};
  vector_t n = normalizeVector(posVec);
  vector_t t = rotate2d(n, M_PI / 2);

  vector_t velVec = {ball1->velocity.x - ball2->velocity.x, ball1->velocity.y - ball2->velocity.y};
  double velMagni = magnitudeOf(velVec);

  double ang = angleBetween(velVec, posVec);
  double seno = sin(ang);
  double cosseno = cos(ang);

  // TODO: CHECK IF ADDING V2 TO BOTH VELOCITIES ISNT A MISTAKE
  ball1->velocity.x = t.x * velMagni * seno + ball2->velocity.x;
  ball1->velocity.y = t.y * velMagni * seno + ball2->velocity.y;

  ball2->velocity.x = n.x * velMagni * cosseno + ball2->velocity.x;
  ball2->velocity.y = n.y * velMagni * cosseno + ball2->velocity.y;
}

void makeBallsKiss(Ball *ball1, Ball *ball2) {

  vector_t ballBallVec = {ball2->position.x - ball1->position.x, ball2->position.y - ball1->position.y};

  vector_t n = normalizeVector(ballBallVec);

  // TODO - Fix the use of EPS to make it regular
  double correction = 2 * ball1->radius - magnitudeOf(ballBallVec) + 1e-8;

  ball1->position.x += correction / 2 * n.x;
  ball1->position.y += correction / 2 * n.y;

  ball2->position.x -= correction / 2 * n.x;
  ball2->position.y -= correction / 2 * n.y;
}

void resolveBallCushion(Ball *ball, Cushion *cushion, double restitution) {

  if (dotProduct(cushion->normal, ball->velocity) <= 0) {
    cushion->normal.x = -cushion->normal.x;
    cushion->normal.y = -cushion->normal.y;
  }

  makeBallCushionKiss(ball, cushion);

  double ang = angle(cushion->normal);
  vector_t velocityC = rotate2d(ball->velocity, -ang);

  velocityC.x *= -restitution;

  ball->velocity = rotate2d(velocityC, ang);
  ball->state = SLIDING;
}

void makeBallCushionKiss(Ball *ball, Cushion *cushion) {

  vector_t c = linePointClosestTo(cushion->p1, cushion->p2, ball->position);

  // TODO: CREATE FUNCIONS FOR THIS??
  // TODO: FIX USE OF EPS
  vector_t connection = {ball->position.x - c.x, ball->position.y - c.y};
  double correction = ball->radius - magnitudeOf(connection) + 1e-9;

  ball->position.x -= correction * cushion->normal.x;
  ball->position.y -= correction * cushion->normal.y;
}

void resolveBallPocket(Ball *ball, Pocket *pocket) {
  ball->state = POCKETED;
  ball->position = pocket->position;
}

void assertStationary(Ball *ball) {
  // TODO: USE BETTER EPS
  assert(ball->velocity.x < EPS_SPACE);
  assert(ball->velocity.y < EPS_SPACE);

  assert(ball->ang_velocity.x < EPS_SPACE);
  assert(ball->ang_velocity.y < EPS_SPACE);
  assert(ball->ang_velocity.z < EPS_SPACE);

  ball->velocity.x = 0;
  ball->velocity.y = 0;

  ball->ang_velocity.x = 0;
  ball->ang_velocity.y = 0;
  ball->ang_velocity.z = 0;

  ball->state = STATIONARY;
}

void assertSpinning(Ball *ball) {
  // TODO: USE BETTER EPS
  assert(ball->velocity.x > EPS_SPACE);
  assert(ball->velocity.y > EPS_SPACE);

  assert(ball->ang_velocity.x > EPS_SPACE);
  assert(ball->ang_velocity.y > EPS_SPACE);

  ball->velocity.x = 0;
  ball->velocity.y = 0;

  ball->ang_velocity.x = 0;
  ball->ang_velocity.y = 0;

  ball->state = SPINNING;
}

void resolveStickBall(Cue *cue, Ball *ball, double maxSpeed) {


  // TODO: CHECK IF SPIN IS NOT TOO MUCH
  double sideSpin = cue->sideEnglish * ball->radius;
  double verticalSpin = cue->verticalEnglish * ball->radius;

  double phi = cue->angle;
  double theta = cue->elevationAngle;

  // CHECK: II = 2 / 5 * m * R**2 if mass can be removed
  double II = 2.0 / 5.0 * ball->radius * ball->radius;
  double c = sqrt(ball->radius * ball->radius - sideSpin * sideSpin - verticalSpin * verticalSpin);

  // CHECK: numerator = 2 * M * V0 If mass can be removed
  double numerator = 2 * cue->charge * maxSpeed;
  double cosseno = cos(theta);
  double seno = sin(theta);

  double temp = sideSpin * sideSpin + (verticalSpin * cosseno) * (verticalSpin * cosseno) + (c * cosseno) * (c * cosseno) - 2 * verticalSpin * c * cosseno * seno;

  // CHECK denominator = 1 + m / M + 5 / 2 / R**2 * temp
  double denominator = 1 + 5.0 / (2.0 * (ball->radius * ball->radius)) * temp;


  double F = numerator / denominator;

  vector_t velocityB = {0, -F * cosseno};

  vector3_t vec = {-c * seno + verticalSpin * cosseno, sideSpin * seno, -sideSpin * cosseno};
  vector3_t angVelocityB = {F / II * vec.x, F / II * vec.y, F / II * vec.z};



  double ang = phi + M_PI / 2;

  ball->velocity = rotate2d(velocityB, ang);
  vector_t xyAngVelocity = {angVelocityB.x, angVelocityB.y};
  xyAngVelocity = rotate2d(xyAngVelocity, ang);
  ball->ang_velocity.x = xyAngVelocity.x;
  ball->ang_velocity.y = xyAngVelocity.y;
  ball->ang_velocity.z = angVelocityB.z;

  ball->state = SLIDING;

  
}
