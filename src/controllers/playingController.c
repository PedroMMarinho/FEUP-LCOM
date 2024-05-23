

#include "playingController.h"
#include "../labs/graphics.h"
#include "../viewer/cueViewer.h"
#include "../viewer/lineViewer.h"
#include "../xpms/ball.xpm"

STATE playingControllerHandle(Table *table, DEVICE interruptType, const struct packet *packet, uint8_t scanCode, unsigned elapsed) {
  switch (interruptType) {
    case TIMER:
      if (elapsed % (sys_hz() / 30) == 0) {
        if (drawTable(table)){
          return OVER;
        }
        if (swap_buffers()) return 1;
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

          if (!packet->lb){
            if (table->cue->charge) {
              table->state = SIMULATING;
              table->cue->charge = 0;
            }else{
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
      if (scanCode == 0x81)
        return OVER;
      break;
    default:
      break;
  }
  return PLAYING;
}
