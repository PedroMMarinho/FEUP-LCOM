#pragma once

#include <lcom/lcf.h>
#include "vector.h"
#include "ball/ball.h"
#include "mouseModel.h"
#include "cue.h"

typedef enum GAME_STATE {
  AIMING,
  SHOOTING,
  WAITING,
  SIMULATING,
} GAME_STATE;

typedef struct Table{
  xpm_image_t img;
  vector_t* cushionPoints;
  vector_t pockets[4];
  size_t pocketRadius;
  Ball** balls;
  uint8_t ballNumber;
  GAME_STATE state;
  Mouse* mouse;
  Cue* cue;

  // Physics measurments
  double slidingFriction;
  double spinningFriction;
  double rollingFriction;
  double gravityAcceleration;
  size_t ballRadius;

} Table;


Table * newTable();

void destroyTable(Table* table);

int drawTable(Table* table);

int updateCueState(Table* table, bool power);

bool getColisionPoint(Table* table, vector_t*collisionPoint);
