#pragma once

#include <lcom/lcf.h>

typedef enum STATE {
  PLAYING,
  MENU,
  OVER,
} STATE;


typedef enum DEVICE {
  MOUSE,
  KEYBOARD,
  RTC,
  PORT,
  TIMER,
  SP,
} DEVICE;

