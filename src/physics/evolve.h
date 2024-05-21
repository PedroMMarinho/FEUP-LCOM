
#pragma once
#include <lcom/lcf.h>
#include "../model/ball/ball.h"


void evolveBallMotion(Ball* ball){

  switch (getBallState(ball))
  {
  case STATIONARY: case POCKETED:
    return;
    break;
  case SLIDING: 
    
  break;

  case ROLLING:

  break;

  case SPINNING:

  break;
  default:
    break;
  }

}


