#include <lcom/lcf.h>

typedef enum {

  BALL_BALL,
  BALL_CUSHION,
  BALL_POCKET,
  STICK_BALL,
  INVALID,

}EVENT_TYPE;



typedef struct Event {

  double time;
  EVENT_TYPE type;

  int ball1;
  int ball2;
  int cushionId;
  // TODO: NECESSARY TO SAVE THE POCKET ???
  int pocket;

}Event;
