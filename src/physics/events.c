#include "events.h"
#include "math.h"
#include "utilities.h"

Event getNextBallBallCollision(Table *table) {

  // TODO: CREATE BETTER CONSTRUCTOR ??
  Event event = {INFINITY, INVALID, NULL, NULL, 0, 0};

  size_t collisionNumber = 0;
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
      balls2[collisionNumber] = j;

      coeficients[collisionNumber] = getBallBallCollisionCoeff(ball1, ball2, table->rollingFriction, table->slidingFriction, table->gravityAcceleration);
      collisionNumber++;
    }
  }
  if (collisionNumber == 0)
    return event;
  collisionNumber--;

  double collisionTime;
  int i = findSmallerCoeficient(collisionNumber, coeficients, &collisionTime);
  if (i == -1)
    return event;

  event.type = BALL_BALL;
  event.ball1 = table->balls[balls1[i]];
  event.ball2 = table->balls[balls2[i]];
  event.time = collisionTime;

  free(coeficients);
  free(balls1);
  free(balls2);
  return event;
}

Event getNextBallCushionCollision(Table *table) {
  // TODO: CREATE BETTER CONSTRUCTOR ??
  Event event = {INFINITY, INVALID, NULL, NULL, 0, 0};

  for (size_t i = 0; i < table->ballNumber; i++) {

    Ball *ball = table->balls[i];

    if (ballNotMoving(ball))
      continue;

    // TODO: is the hard code ok in this case?
    for (size_t j = 0; j < 23; j++) {

      Cushion* cushion = table->cushions[j];

      double collisionTime = getBallCushionCollisionTime(table, ball, cushion);

      if (collisionTime < event.time) {
        event.time = collisionTime;
        event.type = BALL_CUSHION;
        event.ball1 = table->balls[i];
        event.cushion = cushion;
      }
    }
  }

  return event;
}

Event getNextBallPocketCollision(Table *table) {

  // TODO: CREATE BETTER CONSTRUCTOR ??
  Event event = {INFINITY, INVALID, NULL, NULL, 0, 0};

  size_t *balls = (size_t *) malloc(sizeof(size_t) * table->ballNumber);

  // TODO: CHECK IF POCKET NEEDS TO BE STORED
  size_t *pockets = (size_t *) malloc(sizeof(size_t) * table->ballNumber);
  QuarticCoeff *coeficients = (QuarticCoeff *) malloc(sizeof(QuarticCoeff) * table->ballNumber);

  size_t collisionNumber = 0;

  for (size_t i = 0; i < table->ballNumber; i++) {

    Ball *ball = table->balls[i];

    if (ballNotMoving(ball))
      continue;

    for (size_t j = 0; j < 6; j++) {
      Pocket* pocket = table->pockets[j];
      balls[collisionNumber] = i;
      pockets[collisionNumber] = j;
      coeficients[collisionNumber] = getBallPocketCollisionCoeff(ball, pocket, table->rollingFriction, table->slidingFriction, table->gravityAcceleration);
      collisionNumber++;
    }
  }
  if (collisionNumber == 0) {
    return event;
  }
  collisionNumber--;

  double collisionTime;
  int i = findSmallerCoeficient(collisionNumber, coeficients, &collisionTime);
  if (i == -1)
    return event;

  event.type = BALL_POCKET;
  event.ball1 = table->balls[balls[i]];
  event.pocket = pockets[i];
  event.time = collisionTime;

  free(coeficients);
  free(balls);
  free(pockets);
  return event;
}

Event getNextTransition(Table *table) {

  Event nextTransition = {INFINITY, INVALID, NULL, NULL, NULL, -1};

  for (size_t i = 0; i < table->ballNumber; i++) {
    Ball *ball = table->balls[i];
    if (ball->transition->type == INVALID)
      continue;
    if (ball->transition->time < nextTransition.time) {
      nextTransition = *ball->transition;
    }
  }

  return nextTransition;
}

Event getNextEvent(Table *table) {

  Event event = {INFINITY, INVALID, NULL, NULL, NULL, -1};
  Event testEvent;

  testEvent = getNextTransition(table);
  event = testEvent.time < event.time ? testEvent : event;

  testEvent = getNextBallBallCollision(table);
  event = testEvent.time < event.time ? testEvent : event;

  testEvent = getNextBallCushionCollision(table);
  event = testEvent.time < event.time ? testEvent : event;

  testEvent = getNextBallPocketCollision(table);
  event = testEvent.time < event.time ? testEvent : event;

  return event;
}

void updateBallNextTransition(Table *table, Ball *ball) {

  if (ballNotMoving(ball)) {
    Event transition = {0, INVALID, NULL, NULL, NULL, -1};
    *ball->transition = transition;
    return;
  }

  switch (ball->state) {

    case SPINNING: {
      double time = getSpinTime(ball, table->spinningFriction, table->gravityAcceleration);
      Event transition = {time, SPINNING_STATIONARY, ball, NULL, NULL, -1};
      *ball->transition = transition;
      break;
    }
    case ROLLING: {

      double rollTime = getRollTime(ball, table->rollingFriction, table->gravityAcceleration);
      double spinTime = getSpinTime(ball, table->spinningFriction, table->gravityAcceleration);
      Event transition = {rollTime, ROLLING_STATIONARY, ball, NULL, NULL, -1};
      if (spinTime > rollTime) {
        transition.type = ROLLING_SPINNING;
      }
      *ball->transition = transition;
      break;
    }

    case SLIDING: {

      double slideTime = getSlideTime(ball, table->slidingFriction, table->gravityAcceleration);
      Event transition = {slideTime, SLIDING_ROLLING, ball, NULL, NULL, -1};
      *ball->transition = transition;
      break;
    }
    default:
      break;
  }
}
