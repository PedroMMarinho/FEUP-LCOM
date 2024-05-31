#pragma once

#include <lcom/lcf.h>
#include "vector.h"
#include "ball/ball.h"
#include "mouseModel.h"
#include "cue.h"
#include "cushion.h"
#include "pocket.h"

typedef enum GAME_STATE {
  AIMING,
  SHOOTING,
  WAITING,
  SIMULATING,
} GAME_STATE;

typedef struct Table{
  xpm_image_t img;
  Cushion* cushions[18];
  Pocket* pockets[6];
  Ball** balls;
  uint8_t ballNumber;
  GAME_STATE state;
  Mouse* mouse;
  Cue* cue;
  double maxSpeedShot;

  // Physics measurments
  double slidingFriction;
  double spinningFriction;
  double rollingFriction;
  double gravityAcceleration;
  double cushionRestitution;
  size_t ballRadius;


  // Simulation
  Event nextEvent;

} Table;


Table * newTable();

void destroyTable(Table* table);

int drawTable(Table* table);

int updateCueState(Table* table, bool power);

bool getColisionPoint(Table* table, vector_t*collisionPoint);
