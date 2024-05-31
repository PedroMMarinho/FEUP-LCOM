#pragma once
#include <lcom/lcf.h>
#include "vector.h"


typedef struct Cushion{

  vector_t p1;
  vector_t p2;

  double lx;
  double ly;
  double l0;

  vector_t normal;

}Cushion;


Cushion* newCushion(vector_t p1, vector_t p2);
