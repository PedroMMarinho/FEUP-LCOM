
#pragma once
#include <lcom/lcf.h>
#include "../model/ball/ball.h"
#include "../model/table.h"


void evolveBallMotion(Table* table, Ball* ball, double time);

void evolveSlideState(Ball* ball, double t, double uSpinning, double uSliding, double g);

void evolveRollState(Ball* ball, double t, double uRolling, double uSliding, double g);

void evolvePrependicularSpin(Ball* ball, double t , double uSpinning, double g);
