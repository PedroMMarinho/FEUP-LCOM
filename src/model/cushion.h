#pragma once
#include <lcom/lcf.h>
#include "vector.h"

struct Ball;

typedef struct LinearCushion{

  vector_t p1;
  vector_t p2;

  double lx;
  double ly;
  double l0;

  vector_t normal;

}LinearCushion;


typedef struct CircularCushion{

  vector_t position;
  double radius;

} CircularCushion;


LinearCushion* newLinearCushion(vector_t p1, vector_t p2,vector_t normal);

void fixNormalDirection(LinearCushion* cushion,struct Ball* ball);

CircularCushion* newCircularCushion(vector_t pos, double radius);

vector_t CircularCushionNormal(CircularCushion* cushion,struct Ball* ball);
