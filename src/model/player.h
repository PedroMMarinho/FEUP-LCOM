#pragma once
#include <lcom/lcf.h>
#include "vector.h"
#include "../labs/graphics.h"

typedef enum playerBall{
  PLAYERSOLID,
  PLAYERSTRIPED,
  PLAYERBALLNONE,
} playerBall;


typedef struct Player{
  char* name;
  playerBall ballType;
  bool isPlaying;
} Player;


Player* newPlayer(char* name, playerBall ballType , bool isPlaying);

void destroyPlayer(Player* player);

int drawInGamePlayerName(Player* player, xpm_image_t* font, int x, int y, int spacing);
