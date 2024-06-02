#pragma once

#include <lcom/lcf.h>
#include <stdbool.h>
#include "vector.h"
#include "ball/ball.h"
#include "mouseModel.h"
#include "cue.h"
#include "cushion.h"
#include "pocket.h"
#include "player.h"

typedef enum GAME_STATE {
  AIMING,
  SHOOTING,
  SIMULATING,
  ADVANTAGE,
} GAME_STATE;

typedef enum MULTIPLAYER_STATE {
  MULTIWAITING,
  MULTIPLAYING,
  NOTMULTIPLAYER,
} MULTIPLAYER_STATE;

typedef struct Table{
  xpm_image_t img;
  xpm_image_t ui;
  xpm_image_t spinCircle;
  LinearCushion* linearCushions[6];
  CircularCushion* circularCushions[12];
  Pocket* pockets[6];
  Ball** balls;
  uint8_t ballNumber;
  GAME_STATE state;
  MULTIPLAYER_STATE multiplayer;
  Mouse* mouse;
  Cue* cue;
  double maxSpeedShot;
  Player* player1;
  Player* player2;
  Ball* firstBallHit;
  bool pocketedOwnBall;
  // Physics measurments
  double slidingFriction;
  double spinningFriction;
  double rollingFriction;
  double gravityAcceleration;
  double cushionRestitution;
  double cushionFriction;
  size_t ballRadius;
  xpm_image_t* font;

  // Simulation
  Event nextEvent;

} Table;


Table * newTable();

void destroyTable(Table* table);

int drawTable(Table* table,int gameTime, int roundTime);

int updateCueState(Table* table, bool power);

bool getColisionPoint(Table* table, vector_t*collisionPoint);

void glueBall(Table* table);

bool canDropBall(Table* table);

void switchTurn(Table* table);

Player* getPlayingPlayer(Table* table);

Player* getNotPlayingPlayer(Table* table);

bool isPlayerBall(Player* player, Ball* ball);

void drawBalls(Table *table);

bool updateSpin(Table* table);

