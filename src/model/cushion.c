#include "cushion.h"
#include "../physics/utilities.h"

LinearCushion *newLinearCushion(vector_t p1, vector_t p2, vector_t normal) {

  LinearCushion *cushion = (LinearCushion *) malloc(sizeof(LinearCushion));

  cushion->p1 = p1;
  cushion->p2 = p2;
  cushion->normal = normal;

  return cushion;
}

void fixNormalDirection(LinearCushion *cushion, struct Ball *ball) {

  if (dotProduct(cushion->normal, ball->velocity) <= 0) {
    cushion->normal.x = -cushion->normal.x;
    cushion->normal.y = -cushion->normal.y;
  }
}

CircularCushion *newCircularCushion(vector_t pos, double radius) {

  CircularCushion *cushion = (CircularCushion *) malloc(sizeof(CircularCushion));

  cushion->position = pos;
  cushion->radius = radius;
  return cushion;
}

vector_t CircularCushionNormal(CircularCushion *cushion, Ball *ball) {

  vector_t normal = {cushion->position.x - ball->position.x, cushion->position.y - ball->position.y};
  normal = normalizeVector(normal);
  return normal;
}




