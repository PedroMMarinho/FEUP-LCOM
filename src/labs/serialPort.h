#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

#include <lcom/lcf.h>

#include "queue.h"
#include "utils.h"
#include "../model/table.h"
#include "../model/cue.h"


#define COM1 0x3F8
#define COM1_IRQ 4

#define RBR 0x00
#define THR  0x00
#define IER 0x01
#define IIR 0x02
#define FCR 0x02
#define LCR 0x03
#define LSR 0x05


#define DIVISOR_LATCH_LSB 0x00
#define DIVISOR_LATCH_MSB 0x01

#define LCR_8_BITS            (BIT(0) | BIT(1))
#define LCR_ODD_PARITY        BIT(3)
#define LCR_DLAB              BIT(7)

#define LSR_RECEIVER_DATA BIT(0)
#define LSR_OVERRUN_ERROR BIT(1)
#define LSR_PARITY_ERROR BIT(2)
#define LSR_FRAMING_ERROR BIT(3)
#define LSR_BREAK_INTERRUPT BIT(4)
#define LSR_TRANSMITTER_HOLD_EMPTY BIT(5)
#define LSR_TRANSMITTER_EMPTY_REG BIT(6)
#define LSR_FIFO_ERROR BIT(7)

#define IER_RDAI BIT(0)
#define IER_THREI BIT(1)
#define IER_RLSI BIT(2)

#define IIR_PENDING                    BIT(0)
#define IIR_LINE_STATUS                (BIT(2) | BIT(1))
#define IIR_DATA_AVAILABLE             (BIT(2)) 
#define IIR_CHARACTER_TIMEOUT          (BIT(2) | BIT(3)) 
#define IIR_TRANSMIT_HOLD_EMPTY        (BIT(1)) 


#define FCR_ENABLE_FIFO                BIT(0)
#define FCR_CLEAR_RCVR_FIFO            BIT(1)
#define FCR_CLEAR_XMIT_FIFO            BIT(2)


#define TRYSYNC      0x53 
#define FIRSTSYNC    0x54
#define SECONDSYNC   0x55


typedef enum DATATYPES{
  MOUSE_DATA,
  SHOT_DATA,
  BALL_DATA,
} DATATYPES;

typedef union {

  float f;
  uint8_t bytes[4];

} FloatUnion;


int send_SP_data(uint8_t byte);

int (sp_subscribe)(uint8_t *bit_no);

int (sp_unsubscribe)();

int establish_connection();

void sp_ih();

int sp_clear();

void resetMultiplayer();

int handleMultiplayerData(Table* table);

bool player1();

void sendMouseData(Mouse *mouse);

void sendShotData(Cue *cue);

void sendCueBallData(vector_t ballPos);





#endif
