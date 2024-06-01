#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

#include <lcom/lcf.h>

#include "queue.h"
#include "utils.h"



#define COM1 0x3F8
#define COM1_IRQ 4

#define RBR 0x00
#define THR  0x00
#define IER 0x01
#define IIR 0x02
#define FCR 0x02
#define LCR 0x03
#define MCR 0x04
#define LSR 0x05
#define MSR 0x06
#define SR	 0x07 	// Scratchpad Register (Read/Write)

#define DIVISOR_LATCH_LSB 0x00
#define DIVISOR_LATCH_MSB 0x01

#define LCR_5_BITS            (~BIT(0) & ~BIT(1))
#define LCR_6_BITS            (BIT(0) & ~BIT(1))
#define LCR_7_BITS            (~BIT(0) & BIT(1))
#define LCR_8_BITS            (BIT(0) & BIT(1))
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
#define IIR_LINE_STATUS                (~BIT(1) & BIT(2) & BIT(3)) 
#define IIR_DATA_AVAILABLE             (~BIT(1) & BIT(2) & ~BIT(3)) 
#define IIR_CHARACTER_TIMEOUT          (BIT(1) & BIT(2) & ~BIT(3)) 
#define IIR_TRANSMIT_HOLD_EMPTY        (~BIT(1) & ~BIT(2) & BIT(3)) 


#define FCR_ENABLE_FIFO                BIT(0)
#define FCR_CLEAR_RCVR_FIFO            BIT(1)
#define FCR_CLEAR_XMIT_FIFO            BIT(2)


#define ACK 0xFE
#define NACK 0xDE
#define END 0x40

int send_byte(uint8_t byte);

int (sp_subscribe)(uint8_t *bit_no);

int (sp_unsubscribe)();

int establish_connection();

bool get_multiplayer();

void sp_ih();

int sp_clear();





#endif
