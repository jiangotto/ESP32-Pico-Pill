#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define PIN_BACK 21
#define PIN_UP 22
#define PIN_SELECT 19
#define PIN_DOWN 23

#define LV_KEY_ESC_t 27
#define LV_KEY_LEFT_t 20
#define LV_KEY_ENTER_t 10
#define LV_KEY_RIGHT_t 19

void Button_init(void);
int Button_lvgl_cb(void);

#endif