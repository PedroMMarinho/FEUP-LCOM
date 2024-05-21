#pragma once

#include "lcom/lcf.h"
#include "model/table.h"
#include "States/states.h"



typedef struct Resources
{
  Table* table;
  STATE state;
} Resources;
Resources * loadResources();

void destroyResources(Resources* resources);

void deleteResources(Resources* resources);
