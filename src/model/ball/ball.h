
#pragma once
#include <lcom/lcf.h>
#include "../vector.h"


typedef enum BALL_STATE{
  STATIONARY,
  SPINNING,
  SLIDING,
  ROLLING,
  POCKETED,
} BALL_STATE;


typedef  struct Ball
{
  enum BALL_STATE state;
  vector_t position;
  vector_t velocity;
  vector_t ang_velocity;
  xpm_image_t img;
}Ball;

Ball *newBall(vector_t position);

void destroyBall(Ball* ball);

vector_t getBallPosition(Ball* ball);

vector_t getBallVelocity(Ball* ball);

vector_t getBallAngVelocity(Ball* ball);

BALL_STATE getBallState(Ball* ball);

xpm_image_t getBallImage(Ball* ball);
