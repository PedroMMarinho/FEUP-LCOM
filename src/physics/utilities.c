
#include "utilities.h"
#include "math.h"

vector_t unitVector(vector_t start, vector_t end) {
  vector_t vector;
  vector.x = end.x - start.x;
  vector.y = end.y - start.y;
  return normalizeVector(vector);
}

vector_t normalizeVector(vector_t vector) {
  double magnitude = sqrt(vector.x * vector.x + vector.y * vector.y);
  vector.x /= magnitude;
  vector.y /= magnitude;
  return vector;
}

double dotProduct(vector_t a, vector_t b){
  return a.x * b.x + a.y * b.y;
}

