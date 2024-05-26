#pragma once
#include <lcom/lcf.h>
#include "../model/vector.h"
#include "../model/table.h"
#include "../model/ball/ball.h"



vector_t unitVector(vector_t a, vector_t b);

double angle(vector_t vec1);

vector_t normalizeVector(vector_t vector);

double dotProduct(vector_t a, vector_t b);

double magnitudeOf(vector_t vector);

vector_t rotate2d(vector_t vec, double ang);
// Physics calculations utilities

vector_t relativeVelocity(Ball* ball);

double getSlideTime(Ball* ball, double u, double g);
