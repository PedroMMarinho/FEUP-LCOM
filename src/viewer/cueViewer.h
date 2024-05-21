
#pragma once
#include "../model/vector.h"
#include "../model/table.h"

int drawCue(Table* table);

int drawCueCharged(vector_t ball, vector_t origin, int16_t deltaX, int16_t deltaY);

int drawLine(vector_t start, vector_t end);


void plotLineWidth(int x0, int y0, int x1, int y1, float wd);
