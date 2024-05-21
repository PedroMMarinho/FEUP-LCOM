#ifndef MOUSE_H
#define MOUSE_H


#include <stdint.h>


void (mouse_ih)();

bool mouseError();

int(mouse_subscribe)(uint8_t *bit_no);

int(mouse_unsubscribe)(); 

uint8_t get_mouse_byte();

int complete_packet();

bool isPacketComplete();

struct packet getMousePacket();

int updatePacket(uint8_t mouseByte);

int disableDataReporting();

#endif
