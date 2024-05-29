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
#include "xpms/menu/menuMouseHover.xpm"
#include "xpms/menu/mainMenuBackground.xpm"




Menu* newMenu(MenuType type){
  Menu* menu = (Menu*)malloc(sizeof(Menu));
  setMenuType(menu,type);
  menu->mouse = newMouse();
  xpm_image_t menuMouse;
  xpm_image_t menuMouseHover;
  xpm_load(menuMouseXpm, XPM_8_8_8, &menuMouse);
  xpm_load(menuMouseHoverXpm, XPM_8_8_8, &menuMouseHover);
  menu->mouseImg = menuMouse;
  menu->mouseImgHover = menuMouseHover;
  return menu;
}
void setMenuType(Menu* menu, MenuType type){
  menu->type = type;
  
  xpm_image_t menuBackground;
  
  vector_t pos = {350,500};
  vector_t size = {200,100};
  xpm_image_t gameModeMenuLocalSelected;
  xpm_image_t gameModeMenuOnlineSelected;
  xpm_image_t backSelected;
  xpm_image_t mainMenuPlaySelected;
  xpm_image_t mainMenuInfoSelected;
  xpm_image_t mainMenuExitSelected;
  
  xpm_load(mainMenuBackgroundXpm, XPM_8_8_8, &menuBackground);
  menu->backgroundImg = menuBackground;
  switch (type)
  {
  case MAIN_MENU:
    
     menu->nOptions = 3;
    menu->buttons = (Button**)malloc(sizeof(Button*)*menu->nOptions);    
    xpm_load(mainMenuPlaySelectedXpm, XPM_8_8_8, &mainMenuPlaySelected);
    xpm_load(mainMenuInfoSelectedXpm, XPM_8_8_8, &mainMenuInfoSelected);
    xpm_load(mainMenuExitSelectedXpm, XPM_8_8_8, &mainMenuExitSelected);
    
  
    menu->buttons[0] = newButton(mainMenuPlaySelected,pos,size);
    pos.x = 650;
    menu->buttons[1] = newButton(mainMenuInfoSelected,pos,size);
    pos.y = 670;
    pos.x = 450;
    menu->buttons[2] = newButton(mainMenuExitSelected,pos,size);
    
   
    break;
  case GAME_MODE_MENU:
    
    menu->nOptions = 3;
      menu->buttons = (Button**)malloc(sizeof(Button*)*menu->nOptions);
      xpm_load(gameModeMenuLocalSelectedXpm, XPM_8_8_8, &gameModeMenuLocalSelected);
      xpm_load(gameModeMenuOnlineSelectedXpm, XPM_8_8_8, &gameModeMenuOnlineSelected);
      xpm_load(backSelectedXpm, XPM_8_8_8, &backSelected);
      menu->buttons[0] = newButton(gameModeMenuLocalSelected,pos,size);
      pos.x = 650;
      menu->buttons[1] = newButton(gameModeMenuOnlineSelected,pos,size);
      pos.y = 670;
      pos.x = 450;
      menu->buttons[2] = newButton(backSelected,pos,size);
    
      
      break;
  case INSTRUCTIONS_MENU:
  
    menu->nOptions = 1;
      menu->buttons = (Button**)malloc(sizeof(Button*)*menu->nOptions);
      xpm_load(backSelectedXpm, XPM_8_8_8, &backSelected);
      menu->buttons[0] = newButton(backSelected,pos,size);
  
      
      break;
  case GAME_OVER_MENU:
       
      menu->nOptions = 1;
      menu->buttons = (Button**)malloc(sizeof(Button*)*menu->nOptions);
      xpm_load(backSelectedXpm, XPM_8_8_8, &backSelected);
      menu->buttons[0] = newButton(backSelected,pos,size);
      
      
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
int drawMenu(Menu* menu){
  if( drawMenuBackground(menu->backgroundImg)) return 1;
  if(menu->selectedOption != -1){
    if(drawMenuButton(menu->buttons[menu->selectedOption])) return 1;
  }
  if(drawMenuMouse(menu->mouse,menu->mouseImg,menu->mouseImgHover,menu->selectedOption != -1)) return 1;
  return 0;
}
int seeIfMouseIsOverOption(Menu* menu, int* option){ 
  
  for( int i = 0; i < menu->nOptions; i++){
    if(isMouseOverButton(menu->buttons[i],menu->mouse->pos)){
      *option = i;
      return 0;
    }
  }
  *option = -1;
  return 0;
}
void updateSelectedOption(Menu* menu, int option){

  for( int i = 0; i < menu->nOptions; i++){
    bool selected = i == option;
    menu->buttons[i]->selected = selected;
  }
  menu->selectedOption = option;
}