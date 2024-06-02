#pragma once
#include <lcom/lcf.h>
#include "../model/table.h"
#include "../model/event.h"




void processShot(Table* table);

bool updatePhysics(Table* table, double dt);

void evolveBalls(Table *table, double time);
