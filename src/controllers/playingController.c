

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

            if (table->firstBallHit == NULL) { // no ball was hit
              switchTurn(table);
              set_round_time(40);
              table->state = ADVANTAGE;
              printf("Player %d didn't hit any ball\n", getPlayingPlayer(table)->isPlaying);
            }
            else {
              if (getPlayingPlayer(table)->ballType != PLAYERBALLNONE) {
              if (((table->firstBallHit->type != STRIPED) && (getPlayingPlayer(table)->ballType == PLAYERSTRIPED)) || ((table->firstBallHit->type != SOLID) && (getPlayingPlayer(table)->ballType == PLAYERSOLID))) { // player hit the wrong ball
                if (table->balls[1]->state == POCKETED) {
                  return MENU; // player lose
                }
                else {
                  switchTurn(table);
                  set_round_time(40);
                  table->state = ADVANTAGE;
                  printf("Player %d hit the wrong ball\n", getPlayingPlayer(table)->isPlaying);
                }
              }
              else {
                if (table->balls[1]->state == POCKETED) {
                  for (int i = 0; i < table->ballNumber; i++) {
                    if (table->balls[i]->state != POCKETED) {
                      if (((table->balls[i]->type == STRIPED) && (getPlayingPlayer(table)->ballType == PLAYERSTRIPED)) || ((table->balls[i]->type == SOLID) && (getPlayingPlayer(table)->ballType == PLAYERSOLID))) {
                        return MENU; // player lose
                      }
                    }
                  }
                  return MENU; // player win
                }
                else {
                  if (table->balls[0]->state == POCKETED) {
                    switchTurn(table);
                    set_round_time(40);
                    table->state = ADVANTAGE;
                  }
                  else {
                    if (!table->pocketedOwnBall) {
                      switchTurn(table);
                    }
                    set_round_time(40);
                  }
                }
              }
            }
            else {
              if (table->balls[1]->state == POCKETED) {
                return MENU; // player lose
              }
              else {
                if (table->balls[0]->state == POCKETED) {
                  switchTurn(table);
                  set_round_time(40);
                  table->state = ADVANTAGE;
                }
                else {
                  int solidBalls = 0;
                  int stripedBalls = 0;
                  for (int i = 0; i < table->ballNumber; i++) {
                    if (table->balls[i]->state == POCKETED) {
                      if (table->balls[i]->type == SOLID) {
                        solidBalls++;
                      }
                      else if (table->balls[i]->type == STRIPED) {
                        stripedBalls++;
                      }
                    }
                  }
                  if (solidBalls != 0 || stripedBalls != 0) {
                    if (solidBalls > stripedBalls) {
                      getPlayingPlayer(table)->ballType = PLAYERSOLID;
                      getNotPlayingPlayer(table)->ballType = PLAYERSTRIPED;
                    }
                    else if (stripedBalls >= solidBalls) {
                      getPlayingPlayer(table)->ballType = PLAYERSTRIPED;
                      getNotPlayingPlayer(table)->ballType = PLAYERSOLID;
                    }
                  }
                }
              }
            }
            }
            
            if(table->state != ADVANTAGE){
              table->state = AIMING;
            }
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
            table->mouse->savedPos = table->mouse->pos;
            table->state = SHOOTING;
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
        case ADVANTAGE:

          glueBall(table);
          if (packet->lb && canDropBall(table)) {

            table->state = AIMING;
          }
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
