#pragma once
#include <lcom/lcf.h>

int mapMemory(uint16_t mode);
int changeMode(uint16_t mode);


int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);


int vg_draw_vline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);


int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);


int draw_pixel(uint16_t x, uint16_t y, uint32_t color);

int vg_draw_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step);

uint8_t R(uint32_t color);
uint8_t G(uint32_t color);
uint8_t B(uint32_t color);


int drawXPMImage(xpm_image_t img, double x, double y, double angle);

int (cleanCanvas)();
