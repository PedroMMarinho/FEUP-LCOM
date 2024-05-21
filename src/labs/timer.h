#ifndef TIMER_H
#define TIMER_H


#include <stdint.h>

int(timer_subscribe_int)(uint8_t *bit_no);

int(timer_unsubscribe_int)();

void(timer_int_handler)();

int get_elapsed();

int(timer_get_conf)(uint8_t timer, uint8_t *st);

int(timer_set_frequency)(uint8_t timer, uint32_t freq);

void reset_elapsed();

#endif
