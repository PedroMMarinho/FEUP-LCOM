#pragma once
#include "lcom/lcf.h"
#include "../States/states.h"
#include "../model/menu.h"

STATE menuControllerHandle(Menu* menu, DEVICE interruptType, const struct packet *packet, uint8_t scanCode, unsigned elapsed);
