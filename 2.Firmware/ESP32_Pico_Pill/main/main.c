#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_freertos_hooks.h"
#include "esp_system.h"
#include "LCD.h"
#include "Button.h"
#include "lvgl.h"
//#include "ZJUnet.h"

#define LV_TICK_PERIOD_MS 1

static void lv_tick_task(void *arg);
static void guiTask(void *pvParameter);

SemaphoreHandle_t xGuiSemaphore;

void app_main(void)
{
    /*freertos任务*/
    xTaskCreatePinnedToCore(guiTask, "gui", 4096*10, NULL, 1, NULL, 0);
}

static void guiTask(void *pvParameter)
{
    (void) pvParameter;
    xGuiSemaphore = xSemaphoreCreateMutex();

    /*初始化*/
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    lv_app(g);

    while(1) 
    {
        vTaskDelay(5);
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
            lv_task_handler();
            xSemaphoreGive(xGuiSemaphore);
        }
        //lv_timer_handler();
    }
    vTaskDelete(NULL);
}

static void lv_tick_task(void *arg) 
{
    (void) arg;

    lv_tick_inc(LV_TICK_PERIOD_MS);
}