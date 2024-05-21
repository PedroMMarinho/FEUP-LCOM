#pragma once

#include "lcom/lcf.h"
#include "../States/states.h"
#include "../model/table.h"

STATE playingControllerHandle(Table* table, DEVICE interruptType, const struct packet *packet, uint8_t scanCode, unsigned elapsed);
