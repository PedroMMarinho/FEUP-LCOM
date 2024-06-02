#pragma once
#include <lcom/lcf.h>
#include "../model/vector.h"
#include "../model/table.h"
#include "../model/ball/ball.h"
#include "../model/cushion.h"
#include "../model/pocket.h"
#include "../model/event.h"



typedef struct {
  double a, b, c, d, e;
  bool valid;
} QuarticCoeff;
// DEBUGGING

void printEvent(Event* event);

void printFloat(double num);

void printCoef(QuarticCoeff q);

void printVector(vector_t vec);
// Maths

vector_t unitVector(vector_t a, vector_t b);

double angle(vector_t vec1);

double angleBetween(vector_t vec1, vector_t vec2);

vector_t linePointClosestTo(vector_t p1, vector_t p2, vector_t p);

vector_t normalizeVector(vector_t vector);

double dotProduct(vector_t a, vector_t b);

double magnitudeOf(vector_t vector);

vector_t rotate2d(vector_t vec, double ang);

int findSmallerCoeficient(size_t n, QuarticCoeff* coeficients, double* result);

vector_t get_uVec(Ball* ball, double ang);

// Physics calculations utilities

vector_t relativeVelocity(Ball* ball);

double getSlideTime(Ball* ball, double u, double g);

double getRollTime(Ball* ball, double u, double g);

double getSpinTime(Ball* ball, double u, double g);

double getBallLinearCushionCollisionTime(Table *table, Ball *ball, LinearCushion* cushion);

QuarticCoeff getBallBallCollisionCoeff(Ball* ball1, Ball* ball2, double uRolling, double uSlidding, double g);

QuarticCoeff getBallPocketCollisionCoeff(Ball *ball, Pocket* pocket, double uRolling, double uSlidding, double g);

QuarticCoeff getBallCircularCushionCollisionCoeff(Ball *ball, CircularCushion* cushion, double uRolling, double uSlidding, double g);
