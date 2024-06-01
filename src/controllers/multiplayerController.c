#include "multiplayerController.h"
#include "../labs/serialPort.h"


bool multiplayer = false;

STATE handleMultiplayerConnection() {
  if(establish_connection()){
    multiplayer = true;
    return PLAYING;
  }
  return MENU;
}


bool get_multiplayer(){
  return multiplayer;
}
