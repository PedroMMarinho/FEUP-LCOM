#include "simulate.h"
#include "events.h"
#include "evolve.h"
#include "resolve/resolver.h"
#include "utilities.h"

void processShot(Table *table) {
  resolveStickBall(table->cue, table->balls[0], table->maxSpeedShot);
}

bool updatePhysics(Table *table, double dt) {

  int nonMoving = 0;
  // TODO2 ONLY CONSIDER BALLS ACTIVE
  for (size_t i = 0 ; i<table->ballNumber; i++){
    Ball* ball = table->balls[i];

    if (ballNotMoving(ball)) nonMoving++;

    evolveBallMotion(table, ball, dt);
    // ONLY CONSIDER BALLS ACTIVE
    for (size_t j = i + 1 ; j < table->ballNumber; j++){
      Ball* ball2 = table->balls[j];
      solveBallCollision(ball, ball2);
    }
    for (size_t j = 0; j<6; j++){
      Pocket* pocket = table->pockets[j];
      solvePocket(table, ball, pocket);
    }
    for (size_t j = 0; j<6; j++){
      LinearCushion* cushion = table->linearCushions[j];
      if(solveLinearCushion(table, ball, cushion)) break;
    }
    for (size_t j = 0; j<12; j++){
      CircularCushion* cushion = table->circularCushions[j];
      solveCircularCushion(table, ball, cushion);
    }
  }
  printf("ball state %d\n", table->balls[0]->state);
  printf("speed:\n");
  printVector(table->balls[0]->velocity);
  if (nonMoving == table->ballNumber) return false;
  return true;
}

void evolveBalls(Table *table, double time) {
  for (size_t i = 0; i < table->ballNumber; i++) {
    evolveBallMotion(table, table->balls[i], time);
  }
}
