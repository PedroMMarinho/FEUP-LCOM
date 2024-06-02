
#pragma once
#include <lcom/lcf.h>
#include "../../model/event.h"
#include "../../model/table.h"
#include "../../model/cushion.h"

void resolveEvent(Table *table, Event event);

void resolveBallBall(Ball* ball1, Ball* ball2);

void resolveTransition(Table* table, Ball* ball);

void resolveBallCushion(Ball* ball, vector_t cushionNormal, double restitution);

void resolveBallCushionRealistic(Ball *ball, vector_t cushionNormal, double restitution, double friction);

void resolveBallPocket(Ball* ball, Pocket* pocket);

void resolveStickBall(Cue* cue, Ball* ball, double maxSpeed);

void makeBallsKiss(Ball* ball1, Ball* ball2);

void makeBallLinearCushionKiss(Ball* ball, LinearCushion* cushion);

void makeBallCircularCushionKiss(Ball* ball, CircularCushion* cushion);

void assertSpinning(Ball *ball);

void assertStationary(Ball *ball);
