#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "LCD.h"
#include "lvgl.h"
#include "../components/lvgl/examples/lv_examples.h"

#define LV_TICK_PERIOD_MS 1

#define DISP_HOR_RES 172
#define DISP_VER_RES 320

static void flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
static void button_read(lv_indev_drv_t * drv, lv_indev_data_t*data);
static void lv_tick_task(void *arg);
static void lvgl_application(void);
static void guiTask(void *pvParameter);

void app_main(void)
{
    /*freertos任务*/
    xTaskCreatePinnedToCore(guiTask, "gui", 4096*2, NULL, 1, NULL, 0);
}

void flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    uint16_t *color;
    unsigned int size = (area->x2-area->x1+1)*(area->y2-area->y1+1);
    color = (uint16_t *)malloc(size*sizeof(uint16_t));
    for(unsigned int i = 0;i<size;i++)
        color[i] = (color_p[i].full>>8) + (color_p[i].full<<8);
    LCD_lvgl_cb(area->x1, area->x2, area->y1, area->y2, color, size);
    free(color);

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}

void button_read(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
    static uint32_t last_btn = 0;   /*Store the last pressed button*/
    int btn_pr = -1;/*my_btn_read();*/     /*Get the ID (0,1,2...) of the pressed button*/
    if(btn_pr >= 0) {               /*Is there a button press? (E.g. -1 indicated no button was pressed)*/
       last_btn = btn_pr;           /*Save the ID of the pressed button*/
       data->state = LV_INDEV_STATE_PRESSED;  /*Set the pressed state*/
    } else {
       data->state = LV_INDEV_STATE_RELEASED; /*Set the released state*/
    }

    data->btn_id = last_btn;            /*Save the last button*/
}

static void guiTask(void *pvParameter)
{
    (void) pvParameter;

    /*初始化*/
    lv_init();
    LCD_init();

    /*初始化缓冲区*/
    /*A static or global variable to store the buffers*/
    static lv_disp_draw_buf_t disp_buf;
    /*Static or global buffer(s). The second buffer is optional*/
    static lv_color_t buf_1[DISP_HOR_RES * DISP_VER_RES / 10];
    static lv_color_t buf_2[DISP_HOR_RES * DISP_VER_RES / 10];
    /*Initialize `disp_buf` with the buffer(s). With only one buffer use NULL instead buf_2 */
    lv_disp_draw_buf_init(&disp_buf, buf_1, buf_2, DISP_HOR_RES*DISP_VER_RES/10);

    /*注册显示驱动程序*/
    static lv_disp_drv_t disp_drv;          /*A variable to hold the drivers. Must be static or global.*/
    lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
    disp_drv.draw_buf = &disp_buf;          /*Set an initialized buffer*/
    disp_drv.flush_cb = flush_cb;        /*Set a flush callback to draw to the display*/
    disp_drv.hor_res = DISP_HOR_RES;                 /*Set the horizontal resolution in pixels*/
    disp_drv.ver_res = DISP_VER_RES;                 /*Set the vertical resolution in pixels*/
    lv_disp_t * disp;
    disp = lv_disp_drv_register(&disp_drv); /*Register the driver and save the created display objects*/

    /*注册输入驱动程序*/
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);      /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_BUTTON;   /*See below.*/
    indev_drv.read_cb = button_read;              /*See below.*/
    /*Register the driver in LVGL and save the created input device object*/
    lv_indev_t * my_indev = lv_indev_drv_register(&indev_drv);

    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    lvgl_application();

    while(1) 
    {
        vTaskDelay(5);
        lv_timer_handler();
    }
}

static void lvgl_application(void)
{
    // /* Get the current screen  */
    // lv_obj_t * scr = lv_disp_get_scr_act(NULL);

    // /*Create a Label on the currently active screen*/
    // lv_obj_t * label1 =  lv_label_create(scr);

    // /*Modify the Label's text*/
    // lv_label_set_text(label1, "Hello\nworld");

    // /* Align the Label to the center
    //  * NULL means align on parent (which is the screen now)
    //  * 0, 0 at the end means an x, y offset after alignment*/
    // lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);

    // /*Create a Label on the currently active screen*/
    // lv_obj_t * label2 =  lv_label_create(scr);

    // /*Modify the Label's text*/
    // lv_label_set_text(label2, "Hello\nZJU");

    // /* Align the Label to the center
    //  * NULL means align on parent (which is the screen now)
    //  * 0, 0 at the end means an x, y offset after alignment*/
    // lv_obj_align(label2, LV_ALIGN_CENTER, 0, 100);

     /*Create a spinner*/
    lv_obj_t * spinner = lv_spinner_create(lv_scr_act(), 1000, 60);
    lv_obj_set_size(spinner, 100, 100);
    lv_obj_center(spinner);
}

static void lv_tick_task(void *arg) 
{
    (void) arg;

    lv_tick_inc(LV_TICK_PERIOD_MS);
}