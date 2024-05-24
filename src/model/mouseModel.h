#pragma once
#include "lcom/lcf.h"
#include "vector.h"


typedef struct Mouse{
  vector_t pos;
  vector_t savedPos;
  vector_t delta;
}Mouse;

Mouse* newMouse();

void destroyMouse(Mouse* mouse);
