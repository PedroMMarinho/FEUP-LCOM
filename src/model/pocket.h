#pragma once
#include <lcom/lcf.h>
#include "vector.h"


typedef struct Pocket{

  vector_t position;
  int radius;

}Pocket;

Pocket* newPocket(vector_t pos, int radius);


