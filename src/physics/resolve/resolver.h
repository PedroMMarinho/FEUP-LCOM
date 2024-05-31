
#pragma once
#include <lcom/lcf.h>
#include "../../model/event.h"
#include "../../model/table.h"
#include "../../model/cushion.h"

void resolveEvent(Table *table, Event event);

void resolveBallBall(Ball* ball1, Ball* ball2);

void resolveTransition(Table* table, Ball* ball);

void resolveBallCushion(Ball* ball, Cushion* cushion, double restitution);

void resolveBallPocket(Ball* ball, Pocket* pocket);

void resolveStickBall(Cue* cue, Ball* ball, double maxSpeed);

void makeBallsKiss(Ball* ball1, Ball* ball2);

void makeBallCushionKiss(Ball* ball, Cushion* cushion);

void assertSpinning(Ball *ball);

void assertStationary(Ball *ball);
