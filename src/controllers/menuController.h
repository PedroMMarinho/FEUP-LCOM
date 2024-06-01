#pragma once
#include "lcom/lcf.h"
#include "../States/states.h"
#include "../model/menu.h"
#include "multiplayerController.h"
STATE menuControllerHandle(Menu* menu,char* playerName, DEVICE interruptType, const struct packet *packet, uint8_t scanCode, unsigned elapsed);
