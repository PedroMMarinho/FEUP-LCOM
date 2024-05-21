
#pragma once

#include <lcom/lcf.h>
#include <stdint.h>


void(kbc_ih)();

bool keyboardIHError();

int(keyboard_subscribe)(uint8_t *bit_no); 

int(keyboard_unsubscribe)(); 

uint8_t getKeyboardScancode(); 
