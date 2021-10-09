#ifndef __LV_APP_H__
#define __LV_APP_H__

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "../lvgl.h"

void lv_app(lv_group_t * g);

#endif