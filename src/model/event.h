#pragma once
#include <lcom/lcf.h>

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
}EVENT_TYPE;



typedef struct Event {

  double time;
  EVENT_TYPE type;

  // TODO: MAYBE CHANGE TO STORE ACTUALL BALLS
  int ball1;
  int ball2;
  int cushionId;
  // TODO: NECESSARY TO SAVE THE POCKET ???
  int pocket;

}Event;
