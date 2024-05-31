#pragma once
#include <lcom/lcf.h>
#include "cushion.h"

struct Ball;

typedef enum {
  BALL_BALL,
  BALL_CUSHION,
  BALL_POCKET,
  STICK_BALL,
  SPINNING_STATIONARY,
  ROLLING_STATIONARY,
  ROLLING_SPINNING,
  SLIDING_ROLLING,
  INVALID,
} EVENT_TYPE;

typedef struct Event {

  double time;
  EVENT_TYPE type;

  struct Ball *ball1;
  struct Ball *ball2;
  Cushion* cushion;
  // TODO: NECESSARY TO SAVE THE POCKET ???
  int pocket;

} Event;
