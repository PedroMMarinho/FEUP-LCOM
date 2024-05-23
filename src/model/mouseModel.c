#include "mouseModel.h"




Mouse* newMouse( vector_t initialPos =  {300, 300}){
  Mouse* mouse = (Mouse*)malloc(sizeof(Mouse));
  mouse->pos  = initialPos;
  return mouse;
}

void destroyMouse(Mouse* mouse){
  free(mouse);
}
