#include "menuController.h"
#include "../labs/graphics.h"
#include "../model/table.h"

STATE handleMainMenu(Menu *menu, int option) {
  printf("option stuff: %d\n", option);
  switch (option) { // 0 - Play, 1 - Instructions, 2 - Exit
    case 0:
      resetMenu(menu);
      setMenuType(menu, GAME_MODE_MENU);
      return MENU;
      break;
    case 1:
      resetMenu(menu);
      setMenuType(menu, INSTRUCTIONS_MENU);
      return MENU;
      break;
    case 2:
      resetMenu(menu);
      destroyMenu(menu);
      return OVER;
      break;
    default:
      return OVER;
  }
}
STATE handleGameModeMenu(Menu *menu, int option) {
  switch (option) { // 0 - Local, 1 - Online, 2 - Back (local and online not implemented)
    case 0:
      resetMenu(menu);
      destroyMenu(menu);
      return PLAYING;
      break;
    case 1:
      resetMenu(menu);
      destroyMenu(menu);
      return PLAYING;
      break;
    case 2:
      resetMenu(menu);
      setMenuType(menu, MAIN_MENU);
      return MENU;
      break;
    default:
      return OVER;
  }
}
STATE handleInstructionsMenu(Menu *menu, int option) {
  switch (option) { // 0 - Back
    case 0:
      resetMenu(menu);
      setMenuType(menu, MAIN_MENU);
      return MENU;
      break;
    default:
      return OVER;
  }
}
STATE handleGameOverMenu(Menu *menu, int option) {
  switch (option) { // 0 - Back
    case 0:
      resetMenu(menu);
      setMenuType(menu, MAIN_MENU);
      return MENU;
      break;
    default:
      return OVER;
  }
}

STATE menuControllerHandle(Menu *menu, DEVICE interruptType, const struct packet *packet, uint8_t scanCode, unsigned elapsed) {
  switch (interruptType) {
    case TIMER:
      if (elapsed % (sys_hz() / 25) == 0) {
        if (drawMenu(menu,elapsed)) {
          printf("Error drawing menu\n");
          return OVER;
        }
        if(swap_buffers()){
          printf("Error swapping buffers\n");
          return OVER;
        }
        
      
        
        
      }
      break;
    case MOUSE:
      // Update mouse position
      menu->mouse->pos.x += packet->delta_x;
      menu->mouse->pos.y -= packet->delta_y;
      if(menu->mouse->pos.x < 0) menu->mouse->pos.x = 0;
      if(menu->mouse->pos.y < 0) menu->mouse->pos.y = 0;
      if(menu->mouse->pos.x > 1024) menu->mouse->pos.x = 1024;
      if(menu->mouse->pos.y > 768) menu->mouse->pos.y = 768;
      int option;

      // TODO: join this two functions into one, so that we don't have to iterate over the buttons twice
      seeIfMouseIsOverOption(menu, &option);
      updateSelectedOption(menu, option);
      
      if (packet->lb) {
        if (menu->selectedOption != -1) {
          switch (menu->type) {
            case MAIN_MENU:
              return handleMainMenu(menu, menu->selectedOption);
              break;
            case GAME_MODE_MENU:
              return handleGameModeMenu(menu, menu->selectedOption);
              break;
            case INSTRUCTIONS_MENU:
              return handleInstructionsMenu(menu, menu->selectedOption);
              break;
            case GAME_OVER_MENU:
              return handleGameOverMenu(menu, menu->selectedOption);
              break;
            default:
              printf("Invalid menu type\n");
              return OVER;
          }
        }
      }
      break;
    case KEYBOARD:
      if (scanCode == 0x81){
        
        return OVER;
      }
      break;
    default:
      return OVER;
      break;

  }
  return MENU;
}
