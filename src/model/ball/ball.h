
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

typedef struct Ball {
  BALL_STATE state;
  vector_t position;
  vector_t velocity;
  vector3_t ang_velocity;
  xpm_image_t img;
  size_t radius;

  Event* transition;
} Ball;

Ball *newBall(vector_t position);

void destroyBall(Ball *ball);

vector_t getBallPosition(Ball *ball);

vector_t getBallVelocity(Ball *ball);

vector3_t getBallAngVelocity(Ball *ball);

BALL_STATE getBallState(Ball *ball);

xpm_image_t getBallImage(Ball *ball);

bool ballNotMoving(Ball *ball);
