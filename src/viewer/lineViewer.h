#pragma once

#include <lcom/lcf.h>
#include "../model/vector.h"


#define LINE_OVERLAP_NONE 0
#define LINE_OVERLAP_MAJOR 0x01
#define LINE_OVERLAP_MINOR 0x02
#define LINE_OVERLAP_BOTH 0x03


int drawAim();

void drawLineOverlap(unsigned int aXStart, unsigned int aYStart, unsigned int aXEnd, unsigned int aYEnd, uint8_t aOverlap,
                     uint32_t aColor);

                  
void drawThickLine(vector_t start, vector_t end, unsigned int aThickness, uint32_t aColor);
