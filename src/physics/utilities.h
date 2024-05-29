#pragma once
#include <lcom/lcf.h>
#include "../model/vector.h"
#include "../model/table.h"
#include "../model/ball/ball.h"

typedef struct {
  double a, b, c, d, e;
} QuarticCoeff;

vector_t unitVector(vector_t a, vector_t b);

double angle(vector_t vec1);

vector_t normalizeVector(vector_t vector);

double dotProduct(vector_t a, vector_t b);

double magnitudeOf(vector_t vector);

vector_t rotate2d(vector_t vec, double ang);

int quartic(double a, double b,double c,double d,double e,double* ans);

double smallerPositiveQuarticRoot(double a, double b, double c, double d, double e);

double cubic(double b,double c,double d);

int findSmallerCoeficient(size_t n, QuarticCoeff* coeficients, double* result);

vector_t get_uVec(Ball* ball, double ang);

// Physics calculations utilities

vector_t relativeVelocity(Ball* ball);

double getSlideTime(Ball* ball, double u, double g);

double getRollTime(Ball* ball, double u, double g);

double getSpinTime(Ball* ball, double u, double g);

QuarticCoeff getBallBallCollisionCoeff(Ball* ball1, Ball* ball2, double uRolling, double uSlidding, double g);

