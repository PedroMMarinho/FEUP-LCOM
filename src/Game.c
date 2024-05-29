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

// Includes for physics testing
#include "physics/utilities.h"

int initGame() {
  Resources *resources = loadResources();
  printf("created resources\n");

  // Testing of quartic solver
  double a, b, c, d, e;
  a = 2;
  b = 1;
  c = -72;
  d = 9;
  e = 66;
  char aString[20];

  double root = smallerPositiveQuarticRoot(a, b, c, d, e);
  sprintf(aString, "%fl", root);
  printf("THE RESULT: %s\n", aString);

  Ball* ball1 = resources->table->balls[0];
  ball1->position.x = 539;
  ball1->position.y = 444;

  ball1->velocity.x = -13;
  ball1->velocity.y = -241;

  ball1->ang_velocity.x = 14;
  ball1->ang_velocity.y = 49;
  ball1->ang_velocity.z = 53;

  ball1->state = ROLLING;



  Ball* ball2 = resources->table->balls[1];

  ball2->position.x = 30;
  ball2->position.y = 30;

  ball2->velocity.x = -4;
  ball2->velocity.y = -2;

  ball2->ang_velocity.x = 7;
  ball2->ang_velocity.y = 8;
  ball2->ang_velocity.z = 5;

  ball2->state = SLIDING;

  QuarticCoeff coef = getBallBallCollisionCoeff(ball1, ball2, resources->table->rollingFriction, resources->table->slidingFriction, resources->table->gravityAcceleration);
  printf("The coef: ");
  sprintf(aString, "%fl", coef.a);
  printf("%s | ", aString);
  sprintf(aString, "%fl", coef.b);
  printf("%s | ", aString);
  sprintf(aString, "%fl", coef.c);
  printf("%s | ", aString);
  sprintf(aString, "%fl", coef.d);
  printf("%s | ", aString);
  sprintf(aString, "%fl", coef.e);
  printf("%s | \n", aString);


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
  printf("Everything finished\n");
  return 0;
}
