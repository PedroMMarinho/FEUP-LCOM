#pragma once
#include <lcom/lcf.h>
#include "mouseModel.h"
#include "vector.h"
#include "button.h"

typedef enum MenuType {
  MAIN_MENU,
  GAME_MODE_MENU,
  INSTRUCTIONS_MENU,
  OPTIONS_MENU,
  GAME_OVER_MENU,
} MenuType;

typedef struct Menu {
  xpm_image_t backgroundImg;
  xpm_image_t mainMenuLogo;
  xpm_image_t *font;
  MenuType type;  
  int nOptions;
  int selectedOption;
  Mouse* mouse;
  xpm_image_t mouseImg;
  xpm_image_t mouseImgHover;
  Button** buttons;
} Menu;


Menu* newMenu(MenuType type);
void destroyMenu(Menu* menu);
void setMenuType(Menu* menu, MenuType type);
void resetMenu(Menu* menu);
int drawMenu(Menu* menu,int time,char* playerName);
int seeIfMouseIsOverOption(Menu* menu,int *option);
void updateSelectedOption(Menu* menu, int option);
