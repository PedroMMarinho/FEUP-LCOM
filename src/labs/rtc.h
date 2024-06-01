#ifndef RTC_H
#define RTC_H

#include <lcom/lcf.h>


#define RTC_IRQ 8
#define RTC_DELAY 20


#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define RTC_MIN_YEAR 2000
#define RTC_DONT_CARE_VALUE BIT(7) | BIT(6)

/* Registers reserved for time-related functionality */
#define RTC_SECONDS 0
#define RTC_SECONDS_ALARM 1
#define RTC_MINUTES 2
#define RTC_MINUTES_ALARM 3
#define RTC_HOUR 4
#define RTC_HOUR_ALARM 5
#define RTC_DAY_OF_WEEK 6
#define RTC_DAY_OF_MONTH 7
#define RTC_MONTH 8
#define RTC_YEAR 9

/* Registers reserved for control of the RTC */
#define RTC_A 10
#define RTC_B 11
#define RTC_C 12
#define RTC_D 13

/* Register A control */
#define RTC_REG_A_UIP BIT(7)
#define RTC_REG_A_RS3 BIT(3) // periodic int rate selector bit 2
#define RTC_REG_A_RS2 BIT(2) // periodic int rate selector bit 1
#define RTC_REG_A_RS1 BIT(1) // periodic int rate selector bit 0

/* Register B control */
#define RTC_REG_B_SET BIT(7)
#define RTC_REG_B_PIE BIT(6)
#define RTC_REG_B_AIE BIT(5)
#define RTC_REG_B_UIE BIT(4)
#define RTC_REG_B_SQWE BIT(3)
#define RTC_REG_B_DM BIT(2)

/* Register C control */
#define RTC_REG_C_IRQF BIT(7)
#define RTC_REG_C_PF BIT(6)
#define RTC_REG_C_AF BIT(5)
#define RTC_REG_C_UF BIT(4)

/* Register D control */
#define RTC_REG_D_VRT BIT(7)


int (get_game_time)();

void (set_game_time)(int time);

int (get_round_time)();

void (set_round_time)(int time);

char* (get_current_time)();

int(rtc_subscribe)(uint8_t *bit_no);

int(rtc_unsubscribe)();

void(rtc_ih)();



#endif

