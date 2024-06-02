

#include "playingController.h"
#include "../labs/graphics.h"
#include "../labs/scancodes.h"
#include "../model/player.h"
#include "../physics/simulate.h"
#include "../viewer/cueViewer.h"
#include "../viewer/lineViewer.h"
#include "../xpms/ball.xpm"
#include "labs/rtc.h"

STATE playingControllerHandle(Table *table, DEVICE interruptType, const struct packet *packet, uint8_t scanCode, unsigned elapsed) {
  switch (interruptType) {
    case TIMER:
      if (elapsed % (sys_hz() / 30) == 0) {

        if (table->state == SIMULATING) {
          if (!updatePhysics(table, 1.0 / 30.0)) {
            if (table->firstCollision == false) {
              table->player1->isPlaying = !table->player1->isPlaying;
              table->player2->isPlaying = !table->player2->isPlaying;
              printf("Player 1 is playing: %d\n", table->player1->isPlaying);
              set_round_time(40);
            }
            if (table->balls[0]->state == POCKETED) {
              table->player1->isPlaying = !table->player1->isPlaying;
              table->player2->isPlaying = !table->player2->isPlaying;
              vector_t pos = {269, 442};
              table->balls[0]->position = pos;
              table->balls[0]->state = STATIONARY;
              set_round_time(40);
              if (table->balls[1]->state == POCKETED) {
                return MENU;
              }
            }
            if (table->balls[1]->state == POCKETED) {
              if (table->player1->isPlaying) {
                if (table->player1->ballType == PLAYERBALLNONE) {
                  return MENU; // Player lost
                }
              }
              else {
                if (table->player2->ballType == PLAYERBALLNONE) {
                  return MENU; // Player lost
                }
              }
              for (int i = 0; i < 16; i++) {
                if (table->balls[i]->type == table->player1->isPlaying ? table->player1->ballType : table->player2->ballType) {
                  if (table->balls[i]->state != POCKETED) {
                    return MENU; // Player lost
                  }
                }
              }
              return MENU; // Player won
            }
            table->state = AIMING;
          }
        }

        if (drawTable(table, get_game_time(), get_round_time())) {
          return OVER;
        }
        if (swap_buffers())
          return 1;
      }
      break;
    case MOUSE:
      // Update mouse position
      table->mouse->pos.x += packet->delta_x;
      table->mouse->pos.y -= packet->delta_y;

      switch (table->state) {
        case AIMING:
          updateCueState(table, false);
          if (packet->lb) {

            if (!updateSpin(table)) {
              table->mouse->savedPos = table->mouse->pos;
              table->state = SHOOTING;
            }
          }
          break;
        case SHOOTING:
          // Update how much mouse moved
          table->mouse->delta.x = packet->delta_x;
          table->mouse->delta.y = -packet->delta_y;
          updateCueState(table, true);
          // Reset mouse movement because its not automatically reseted
          table->mouse->delta.x = 0;
          table->mouse->delta.y = 0;

          if (!packet->lb) {
            if (table->cue->charge) {
              printf("\n\n\n\nStart simuilation\n\n\n\n\n\n\n");
              table->state = SIMULATING;
              processShot(table);
            }
            else {
              table->state = AIMING;
              table->cue->charge = 0;
            }
          }
          break;
        default:
          break;
      }
      break;
    case KEYBOARD:
      printf("Detected keyboard with scanCode: %x\n", scanCode);
      printf("The comparation is %d", scanCode == 0x81);
      if (scanCode == SCANCODE_ESC) {
        return OVER;
      }
      break;

    case RTC:
      if (get_round_time() == 0) {
        set_round_time(40);
        table->player1->isPlaying = !table->player1->isPlaying;
        table->player2->isPlaying = !table->player2->isPlaying;
      }

      break;

    case SP:
      break;

    default:
      break;
  }
  return PLAYING;
}
