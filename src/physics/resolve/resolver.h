
#pragma once
#include <lcom/lcf.h>
#include "../../model/event.h"
#include "../../model/table.h"

void resolveEvent(Table *table, Event event);

void resolveBallBall(Ball* ball1, Ball* ball2);

void resolveTransition(Table* table, Ball* ball);

void resolveBallCushion(Ball* ball, vector_t p1, vector_t p2, double restitution);

void resolveBallPocket(Ball* ball, vector_t pocket);

void resolveStickBall(Cue* cue, Ball* ball, double maxSpeed);

void makeBallsKiss(Ball* ball1, Ball* ball2);

void makeBallCushionKiss(Ball* ball, vector_t p1, vector_t p2, vector_t normal);

void assertSpinning(Ball *ball);

void assertStationary(Ball *ball);
