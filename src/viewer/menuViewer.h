#pragma once
#include <lcom/lcf.h>
#include <math.h>
#include "../model/button.h"
#include "../model/mouseModel.h"

int drawMenuBackground(xpm_image_t img);

int drawMenuButton(Button* button);

int  drawMenuMouse(Mouse* mouse, xpm_image_t mouseImg, xpm_image_t imgMouseHover, bool isHovering);

int drawMenuLogo(xpm_image_t img,int time);

int drawPlayerName(char* playerName, xpm_image_t* playerNameFont, uint16_t x, uint16_t y, uint8_t spacing);

int drawEllipses(xpm_image_t imgBall,int time);
