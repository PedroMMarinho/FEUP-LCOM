#pragma once
#include <lcom/lcf.h>
#include "mouseModel.h"
#include "vector.h"
#include "button.h"

typedef struct Menu {
  xpm_image_t backgroundImg;
  MenuType type;  
  int nOptions;
  int selectedOption;
  Mouse* mouse;
  xpm_image_t mouseImg;
  xpm_image_t mouseImgHover;
  Button** buttons;
} Menu;


typedef enum MenuType {
  MAIN_MENU,
  GAME_MODE_MENU,
  INSTRUCTIONS_MENU,
  GAME_OVER_MENU,
} MenuType;

Menu* newMenu(MenuType type,vector_t prevMousePos,bool win = false);

void destroyMenu(Menu* menu);
void drawMenu(Menu* menu);
int seeIfMouseIsOverOption(Menu* menu,int *option);
void updateSelectedOption(Menu* menu, int option);
