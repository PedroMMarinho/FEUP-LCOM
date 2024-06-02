#include "Game.h"
#include "model/vector.h"
#include "xpms/ball.xpm"

#include "labs/graphics.h"
#include "labs/kbc.h"
#include "labs/keyboard.h"
#include "labs/mouse.h"
#include "labs/timer.h"

#include "viewer/cueViewer.h"
#include "viewer/lineViewer.h"

#include "States/states.h"

#include "controllers/playingController.h"

#include "resources.h"

// testing includes
#include "physics/QuadRootsRevJ.h"
#include "physics/utilities.h"

int initGame() {
  Resources *resources = loadResources();
  printf("created resources\n");


  long double realResults[4];
  long double imgResults[4];
  long double coef[5] = {-1, 3, 98, 0.123, 324};

  int size = QuadCubicRoots(coef, 4, realResults, imgResults);
  printf("REAL\n");
  for (int i = 0 ; i < size; i++){
    printFloat(realResults[i]);
  }
  printf("IMG\n");
  for (int i = 0 ; i < size; i++){
    printFloat(imgResults[i]);
  }

  return gameLoop(resources);
}

int gameLoop(Resources *resources) {
  mapMemory(0x118);
  changeMode(0x118);

  int r;
  message msg;
  int ipc_status;

  mouse_enable_data_reporting();
  uint8_t mouse_iqr;
  if (mouse_subscribe(&mouse_iqr))
    return 1;
  uint8_t keyboard_iqr;
  if (keyboard_subscribe(&keyboard_iqr))
    return 1;
  uint8_t timer_iqr;
  if (timer_subscribe_int(&timer_iqr))
    return 1;

  while (resources->state != OVER) {
    STATE state = resources->state;
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

          if (msg.m_notify.interrupts & timer_iqr) {
            timer_int_handler();
            switch (state) {
              case MAIN_MENU:
                break;
              case PLAYING:
                resources->state = playingControllerHandle(resources->table, TIMER, NULL, 0, get_elapsed());
                break;
              default:
                break;
            }
          }

          if (msg.m_notify.interrupts & mouse_iqr) {
            mouse_ih();
            if (mouseError())
              return 1;
            if (isPacketComplete()) {
              struct packet packet = getMousePacket();
              switch (state) {
                case MAIN_MENU:
                  break;
                case PLAYING:
                  resources->state = playingControllerHandle(resources->table, MOUSE, &packet, 0, 0);
                  break;
                default:
                  break;
              }
            }
          }
          if (msg.m_notify.interrupts & keyboard_iqr) {
            kbc_ih();
            if (keyboardIHError())
              return 1;
            uint8_t scancode = getKeyboardScancode();
            printf("The received scanCode is: %x\n", scancode);
            switch (state) {
              case MAIN_MENU:
                break;
              case PLAYING:
                resources->state = playingControllerHandle(resources->table, KEYBOARD, NULL, scancode, 0);
                break;
              default:
                break;
            }
          }
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
  }
  printf("Closing lab stuff\n");
  if (mouse_unsubscribe())
    return 1;
  if (disableDataReporting())
    return 1;
  if (keyboard_unsubscribe())
    return 1;
  if (timer_unsubscribe_int())
    return 1;

  printf("Exiting video mode\n");
  if (vg_exit())
    return 1;
  printf("Cleaning resources\n");
  destroyResources(resources);
  printf("Everything finished\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  return 0;
}
