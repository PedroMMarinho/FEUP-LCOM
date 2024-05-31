#include "cushion.h"
#include "../physics/utilities.h"


Cushion* newCushion(vector_t p1, vector_t p2){

  Cushion* cushion = (Cushion*)malloc(sizeof(Cushion));

  cushion->p1 = p1;
  cushion->p2 = p2;

  cushion->lx = p2.x - p1.x == 0 ? 1 : -(p2.y - p1.y) / (p2.x - p1.x);
  cushion->ly = p2.x - p1.x == 0 ? 0 : 1;
  cushion->l0 = (p2.x - p1.x) == 0 ? -p1.x : -cushion->lx * p1.x - p1.y;

  vector_t normal = {cushion->lx, cushion->ly};
  cushion->normal = normalizeVector(normal);
  return cushion;
}
