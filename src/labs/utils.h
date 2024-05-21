#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

bool is_make_scancode(uint8_t scancode);

int create_scancode_array(uint8_t size, uint8_t scancode, uint8_t bytes[]);

int(util_sys_inb)(int port, uint8_t *value);

int get_no_sysinb();

int(util_sys_inb)(int port, uint8_t *value);


int(util_get_MSB)(uint16_t val, uint8_t *msb);

int(util_get_LSB)(uint16_t val, uint8_t *lsb);

#endif
