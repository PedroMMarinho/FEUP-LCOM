
#pragma once
#include <lcom/lcf.h>
#include "../vector.h"
#include "../event.h"


typedef enum BALL_STATE {
  STATIONARY,
  SLIDING,
  SPINNING,
  ROLLING,
  POCKETED,
} BALL_STATE;

typedef enum BALL_TYPE {
  STRIPED,
  SOLID,
  BLACK,
  WHITE,
} BALL_TYPE;

typedef struct Ball {
  BALL_STATE state;
  vector_t position;
  vector_t velocity;
  vector3_t ang_velocity;
  xpm_image_t img;
  size_t radius;
  BALL_TYPE type;
  Event* transition;
} Ball;

Ball *newBall(vector_t position,xpm_image_t img,BALL_TYPE type);

void destroyBall(Ball *ball);

vector_t getBallPosition(Ball *ball);

vector_t getBallVelocity(Ball *ball);

vector3_t getBallAngVelocity(Ball *ball);

BALL_STATE getBallState(Ball *ball);

xpm_image_t getBallImage(Ball *ball);

bool ballNotMoving(Ball *ball);
