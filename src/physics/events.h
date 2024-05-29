#pragma once
#include <lcom/lcf.h>
#include "../model/table.h"
#include "../model/event.h"





Event getNextBallBallCollision(Table* table);

Event getNextBallCushionCollision(Table* table);

Event getNextBallPocketCollision(Table* table);
