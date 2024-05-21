#include "Game.h"
#include "vector.h"
#include "ball/ball.h"
#include "../xpms/ball.xpm"
#include "../labs/keyboard.h"
#include "../labs/timer.h"
#include "../labs/kbc.h"
#include "../labs/graphics.h"
#include "../labs/mouse.h"

#include "../viewer/cueViewer.h"
#include "../viewer/lineViewer.h"

int runGame(){

  mapMemory(0x118);
  changeMode(0x118);


  vector_t saveMouse;
  vector_t mouse = {500, 300};
  int16_t deltaX;
  int16_t deltaY;

  int r;
  message msg;
  int ipc_status;
  uint8_t byteNumber = 0;
  struct packet packet;

  enum type {
    AIMING,
    SHOOTING,
    MOVING,
  };
  
  vector_t ballPos = {500, 500};
  Ball* ball = newBall(ballPos);

  enum type state = AIMING;


  mouse_enable_data_reporting();
  uint8_t mouse_iqr;
  if (mouse_subscribe(&mouse_iqr))
    return 1;
  uint8_t keyboard_iqr;
  if (keyboard_subscribe(&keyboard_iqr))
    return 1;
    uint8_t timer_iqr;
    if (timer_subscribe_int(&timer_iqr)) return 1;

  bool terminated = false;
  while (!terminated) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

          if (msg.m_notify.interrupts & timer_iqr){
            timer_int_handler();
            if (get_elapsed() % (sys_hz()/30) == 0){

              if (cleanCanvas()) return 1;
              switch (state)
              {
              case AIMING:
                if (drawCue(getPosition(ball), mouse)) return 1;
                break;
              case SHOOTING:
                if (drawCueCharged(getPosition(ball), saveMouse, deltaX, deltaY)) return 1;
                vg_draw_rectangle(saveMouse.x, saveMouse.y, 8, 8, 0xff0000);
                deltaX = 0;
                deltaY = 0;
              break;
              case MOVING:


              break;
              
              default:
                break;
              }
              vg_draw_rectangle(mouse.x, mouse.y, 9, 9, 0xff0000);
              vg_draw_rectangle(500, 500, 2, 2, 0xfffbc3);
            }
          }
        
          if (msg.m_notify.interrupts & mouse_iqr) {
            mouse_ih();
            if (mouseIHError())
              return 1;
            uint8_t byte = get_mouse_byte();
            packet.bytes[byteNumber++] = byte;

            if (byteNumber == 3) {
              byteNumber = 0;

              // Write all the information
              if (complete_packet(&packet))
                return 1;
                // printf("A new packet has just arrived\n");

              mouse.x += packet.delta_x;
              mouse.y -= packet.delta_y;
              switch (state)
              {
              case AIMING:
                if (packet.lb){
                saveMouse = mouse;
                state = SHOOTING;
                }
                break;
              case SHOOTING:
                deltaX += packet.delta_x;
                deltaY += packet.delta_y;
                if (!packet.lb) state = AIMING;
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

            if (scancode == 0x81)
              terminated = true;
          }
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
  }
  if (mouse_unsubscribe())
    return 1;
  if (disableDataReporting())
    return 1;
  if (keyboard_unsubscribe())
    return 1;
  if (timer_unsubscribe_int()) return 1;

  if (vg_exit()) return 1;

  return 0;

}
