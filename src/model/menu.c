#include "menu.h"


Menu* newMenu(MenuType type,vector_t prevMousePos, bool win){
  Menu* menu = (Menu*)malloc(sizeof(Menu));
  menu->type = type;
    switch (type)
    {
    case MAIN_MENU:
      menu->nOptions = 3;
      menu->buttons = (Button**)malloc(sizeof(Button*)*menu->nOptions);
      menu->buttons[0] = newButton(mainMenuPlay,mainMenuPlaySelected,{400,400},{50,100});
      menu->buttons[1] = newButton(mainMenuInstructions,mainMenuInstructionsSelected,{400,500},{50,100});
      menu->buttons[2] = newButton(mainMenuExit,mainMenuExitSelected,{400,600},{50,100});
      menu->backgroundImg = menuBackground;
      break;
    case GAME_MODE_MENU:
        menu->nOptions = 3;
        menu->buttons = (Button**)malloc(sizeof(Button*)*menu->nOptions);
        menu->buttons[0] = newButton(gameModeMenuLocal,gameModeMenuLocalSelected,{400,400},{50,100});
        menu->buttons[1] = newButton(gameModeMenuOnline,gameModeMenuOnlineSelected,{400,500},{50,100});
        menu->buttons[2] = newButton(gameModeMenuBack,gameModeMenuBackSelected,{400,600},{50,100});
        menu->backgroundImg = gameModeMenuBackground;
        break;
    case INSTRUCTIONS_MENU:
        menu->nOptions = 1;
        menu->buttons = (Button**)malloc(sizeof(Button*)*menu->nOptions);
        menu->buttons[0] = newButton(instructionsMenuBack,instructionsMenuBackSelected,{400,600},{50,100});
        menu->backgroundImg = instructionsMenuBackground;

        break;
    case GAME_OVER_MENU:
        menu->nOptions = 1;
        menu->buttons = (Button**)malloc(sizeof(Button*)*menu->nOptions);
        menu->buttons[0] = newButton(gameOverMenuBack, gameOverMenuBackSelected,{400,600},{50,100});
        menu->backgroundImg = win ? gameOverMenuWinBackground : gameOverMenuLoseBackground;
        break;
    }
  menu->selectedOption = -1;
  menu->mouse = newMouse(prevMousePos);
  return menu;
}
void destroyMenu(Menu* menu){
  free(menu->mouse);
  free(menu);
}
void drawMenu(Menu* menu){
  drawBackground(menu->backgroundImg);
  for(int i = 0; i < menu->nOptions; i++){
    drawButton(menu->buttons[i]);
  }
  drawMouse(menu->mouse);
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