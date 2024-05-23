#include "menu.h"
#include "viewer/menuViewer.h"


#include "xpms/menu/back.xpm"
#include "xpms/menu/backSelected.xpm"
#include "xpms/menu/gameModeMenuLocal.xpm"
#include "xpms/menu/gameModeMenuLocalSelected.xpm"
#include "xpms/menu/gameModeMenuOnline.xpm"
#include "xpms/menu/gameModeMenuOnlineSelected.xpm"
#include "xpms/menu/mainMenuExit.xpm"
#include "xpms/menu/mainMenuExitSelected.xpm"
#include "xpms/menu/mainMenuPlay.xpm"
#include "xpms/menu/mainMenuPlaySelected.xpm"
#include "xpms/menu/menuBackground.xpm"
#include "xpms/menu/mainMenuInfo.xpm"
#include "xpms/menu/mainMenuInfoSelected.xpm"
#include "xpms/menu/menuMouse.xpm"

Menu* newMenu(MenuType type,vector_t prevMousePos, bool win){
  Menu* menu = (Menu*)malloc(sizeof(Menu));
  setMenuType(menu,type);
  menu->mouse = newMouse(prevMousePos);
  menu->mouseImg = menuMouse;
  menu->mouseImgHover = menuMouse;
  return menu;
}
void setMenuType(Menu* menu, MenuType type){
  menu->type = type;
  switch (type)
  {
  case MAIN_MENU:
    menu->nOptions = 3;
    menu->buttons = (Button**)malloc(sizeof(Button*)*menu->nOptions);
    menu->buttons[0] = newButton(mainMenuPlay,mainMenuPlaySelected,{400,400},{50,100});
    menu->buttons[1] = newButton(mainMenuInfo,mainMenuInfoSelected,{400,500},{50,100});
    menu->buttons[2] = newButton(mainMenuExit,mainMenuExitSelected,{400,600},{50,100});
    menu->backgroundImg = menuBackground;
    break;
  case GAME_MODE_MENU:
      menu->nOptions = 3;
      menu->buttons = (Button**)malloc(sizeof(Button*)*menu->nOptions);
      menu->buttons[0] = newButton(gameModeMenuLocal,gameModeMenuLocalSelected,{400,400},{50,100});
      menu->buttons[1] = newButton(gameModeMenuOnline,gameModeMenuOnlineSelected,{400,500},{50,100});
      menu->buttons[2] = newButton(back,backSelected,{400,600},{50,100});
      menu->backgroundImg = menuBackground;
      break;
  case INSTRUCTIONS_MENU:
      menu->nOptions = 1;
      menu->buttons = (Button**)malloc(sizeof(Button*)*menu->nOptions);
      menu->buttons[0] = newButton(back,backSelected,{400,600},{50,100});
      menu->backgroundImg = menuBackground;

      break;
  case GAME_OVER_MENU:
      menu->nOptions = 1;
      menu->buttons = (Button**)malloc(sizeof(Button*)*menu->nOptions);
      menu->buttons[0] = newButton(back, backSelected,{400,600},{50,100});
      menu->backgroundImg = menuBackground;
      break;
  }
  menu->selectedOption = -1;
}
void destroyMenu(Menu* menu){
  free(menu->mouse);
  free(menu);
}
void resetMenu(Menu* menu){
  menu->selectedOption = -1;
  for(int i = 0; i < menu->nOptions; i++){
    destroyButton(menu->buttons[i]);
  }
}
void drawMenu(Menu* menu){
  drawMenuBackground(menu->backgroundImg);
  for(int i = 0; i < menu->nOptions; i++){
    drawMenuButton(menu->buttons[i]);
  }
  drawMenuMouse(menu->mouse,menu->mouseImg,menu->mouseImgHover,menu->selectedOption != -1);
}
void updateSelectedOption(Menu* menu, int option){
  menu->selectedOption = option;
}
int seeIfMouseIsOverOption(Menu* menu, int* option){ 
  
  for( int i = 0; i < menu->nOptions; i++){
    if(isMouseOverButton(menu->buttons[i],menu->mouse->position)){
      *option = i;
      return 0;
      
    }
  }
  *option = -1;
  return 0;
}
int updateSelectedOption(Menu* menu, int option){

  menu->selectedOption = option;
  menu->buttons[menu->selectedOption]->selected = true;
  return 0;
}