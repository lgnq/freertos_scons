#ifndef __DRV_LED_H__
#define __DRV_LED_H__

#include "LPC17xx.h"
#include "lpc17xx_gpio.h"

#define LED0    0
#define LED1    1
#define LED2    2
#define LED3    3

void led_init(void);
void led_on(char led);
void led_off(char led);

#endif

