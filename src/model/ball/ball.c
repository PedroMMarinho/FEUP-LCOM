
#include <lcom/lcf.h>
#include "ball.h"
#include "../../xpms/ball.xpm"



Ball *newBall(vector_t position){
  Ball* ball = (Ball*)malloc(sizeof(Ball));
  ball->position = position;
  ball->radius = 20;
  ball->state = STATIONARY;
  // Set image

  xpm_image_t img;
  xpm_load(ballXpm, XPM_8_8_8, &img);
  ball->img = img;

  return ball;
}

void destroyBall(Ball* ball){
  free(ball);
}


vector_t getBallPosition(Ball* ball){
  return ball->position;
}

vector_t getBallVelocity(Ball* ball){
  return ball->velocity;
}

vector_t getBallAngVelocity(Ball* ball){
  return ball->ang_velocity;
}

BALL_STATE getBallState(Ball* ball){
  return ball->state;
}

xpm_image_t getBallImage(Ball* ball){
  return ball->img;
}
