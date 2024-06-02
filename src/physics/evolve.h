
#pragma once
#include <lcom/lcf.h>
#include "../model/ball/ball.h"
#include "../model/table.h"


void evolveBallMotion(Table* table, Ball* ball, double time);

void solveBallCollision(Ball* ball1, Ball* ball2);

void solvePocket(Table* table, Ball* ball, Pocket* pocket);

bool solveLinearCushion(Table* table, Ball* ball, LinearCushion* cushion);

void solveCircularCushion(Table* table, Ball* ball, CircularCushion* cushion);

void evolveSlideState(Ball* ball, double t, double uSpinning, double uSliding, double g);

void evolveRollState(Ball* ball, double t, double uRolling, double uSliding, double g);

void evolvePrependicularSpin(Ball* ball, double t , double uSpinning, double g);

void solveBallCollision(Ball* ball1, Ball* ball2);

void solvePocket(Table* table, Ball* ball, Pocket* pocket);
