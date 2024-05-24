#pragma once

#include "lcom/lcf.h"
#include "model/table.h"
#include "States/states.h"
#include "model/menu.h"


typedef struct Resources
{
  Table* table;
  Menu* menu;
  STATE state;
} Resources;
Resources * loadResources();

void destroyResources(Resources* resources);

void deleteResources(Resources* resources);
