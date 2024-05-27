
#include "utilities.h"
#include "math.h"

double angle(vector_t vec1) {

  char a[30];
  sprintf(a, "%f", vec1.x);
  printf("First--- %s\n", a);
  sprintf(a, "%f", vec1.y);
  printf("SEcond--- %s\n", a);

  return atan2(vec1.y, vec1.x);
}

vector_t rotate2d(vector_t vec, double ang) {

  double cosseno = cos(ang);
  double seno = sin(ang);

  vector_t rotated = {cosseno * vec.x - seno * vec.y, seno * vec.x + cosseno * vec.y};
  return rotated;
}

vector_t unitVector(vector_t start, vector_t end) {
  vector_t vector;
  vector.x = end.x - start.x;
  vector.y = end.y - start.y;
  return normalizeVector(vector);
}

vector_t normalizeVector(vector_t vector) {
  double magnitude = magnitudeOf(vector);
  vector.x /= magnitude;
  vector.y /= magnitude;
  return vector;
}

double dotProduct(vector_t a, vector_t b) {
  return a.x * b.x + a.y * b.y;
}

double magnitudeOf(vector_t vector) {
  return sqrt(vector.x * vector.x + vector.y * vector.y);
}

// Actually physics calculations
vector_t relativeVelocity(Ball *ball) {

  vector_t relVelocity = {-ball->ang_velocity.y * ball->radius + ball->velocity.x, ball->ang_velocity.x * ball->radius + ball->velocity.y};

  return relVelocity;
}


double getSpinTime(Ball* ball, double u, double g){

  if (u == 0 )return INFINITY;

  return abs(ball->ang_velocity.z) * 2 * ball->radius / (5 * u * g);

}


double getRollTime(Ball* ball, double u, double g){

  if (u == 0) return INFINITY;
  return magnitudeOf(ball->velocity) / (u * g);
}


double getSlideTime(Ball *ball, double u, double g) {
  if (u == 0)
    return INFINITY;

  char a[40];

  double part1 = 2 * magnitudeOf(relativeVelocity(ball));
  double part2 = (7 * u * g);

  sprintf(a, "%f", u);
  printf("u= %s\n", a);
  sprintf(a, "%f", g);
  printf("g= %s\n", a);
  sprintf(a, "%f", part1);
  printf("Part 1 = %s   ", a);
  sprintf(a, "%f", part2);
  printf("Part 2 = %s\n", a);

  return 2 * magnitudeOf(relativeVelocity(ball)) / (7 * u * g);
}
