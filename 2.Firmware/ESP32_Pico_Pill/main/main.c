#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "LCD.h"
#include "Button.h"
#include "lvgl.h"
//#include "ZJUnet.h"
#include "imgbtn_left.h"
#include "imgbtn_mid.h"
#include "imgbtn_right.h"

#define LV_TICK_PERIOD_MS 1

static void lv_tick_task(void *arg);
static void lvgl_application(lv_group_t * g);
static void guiTask(void *pvParameter);

void app_main(void)
{
    /*freertos任务*/
    xTaskCreatePinnedToCore(guiTask, "gui", 4096*2, NULL, 1, NULL, 0);
}

static void guiTask(void *pvParameter)
{
    (void) pvParameter;

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

    lvgl_application(g);

    while(1) 
    {
        vTaskDelay(5);
        lv_timer_handler();
    }
}

static void lv_tick_task(void *arg) 
{
    (void) arg;

    lv_tick_inc(LV_TICK_PERIOD_MS);
}

static void lvgl_application(lv_group_t * g)
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

    // /*Create a spinner*/
    // lv_obj_t * spinner = lv_spinner_create(lv_scr_act(), 1000, 60);
    // lv_obj_set_size(spinner, 100, 100);
    // lv_obj_center(spinner);

    LV_IMG_DECLARE(imgbtn_left);
    LV_IMG_DECLARE(imgbtn_right);
    LV_IMG_DECLARE(imgbtn_mid);

    /*Create a transition animation on width transformation and recolor.*/
    static lv_style_prop_t tr_prop[] = {LV_STYLE_TRANSFORM_WIDTH, LV_STYLE_IMG_RECOLOR_OPA, 0};
    static lv_style_transition_dsc_t tr;
    lv_style_transition_dsc_init(&tr, tr_prop, lv_anim_path_linear, 150, 0, NULL);

    static lv_style_t style_def;
    lv_style_init(&style_def);
    lv_style_set_text_color(&style_def, lv_color_white());
    lv_style_set_transition(&style_def, &tr);

    /*Darken the button when pressed and make it wider*/
    static lv_style_t style_pr;
    lv_style_init(&style_pr);
    lv_style_set_img_recolor_opa(&style_pr, LV_OPA_30);
    lv_style_set_img_recolor(&style_pr, lv_color_black());
    lv_style_set_transform_width(&style_pr, 10);

    /*Create an image button*/
    lv_obj_t * imgbtn1 = lv_imgbtn_create(lv_scr_act());
    lv_imgbtn_set_src(imgbtn1, LV_IMGBTN_STATE_RELEASED, &imgbtn_left, &imgbtn_mid, &imgbtn_right);
    lv_obj_add_style(imgbtn1, &style_def, 0);
    lv_obj_add_style(imgbtn1, &style_pr, LV_STATE_FOCUSED);
    lv_obj_align(imgbtn1, LV_ALIGN_CENTER, 0, -100);

    lv_obj_t * imgbtn2 = lv_imgbtn_create(lv_scr_act());
    lv_imgbtn_set_src(imgbtn2, LV_IMGBTN_STATE_RELEASED, &imgbtn_left, &imgbtn_mid, &imgbtn_right);
    lv_obj_add_style(imgbtn2, &style_def, 0);
    lv_obj_add_style(imgbtn2, &style_pr, LV_STATE_FOCUSED);
    lv_obj_align(imgbtn2, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * imgbtn3 = lv_imgbtn_create(lv_scr_act());
    lv_imgbtn_set_src(imgbtn3, LV_IMGBTN_STATE_RELEASED, &imgbtn_left, &imgbtn_mid, &imgbtn_right);
    lv_obj_add_style(imgbtn3, &style_def, 0);
    lv_obj_add_style(imgbtn3, &style_pr, LV_STATE_FOCUSED);
    lv_obj_align(imgbtn3, LV_ALIGN_CENTER, 0, 100);

    /*Create a label on the image button*/
    lv_obj_t * label1 = lv_label_create(imgbtn1);
    lv_label_set_text(label1, "Click me <3");
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, -4);

    lv_obj_t * label2 = lv_label_create(imgbtn2);
    lv_label_set_text(label2, "Click me <3");
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, -4);

    lv_obj_t * label3 = lv_label_create(imgbtn3);
    lv_label_set_text(label3, "Click me <3");
    lv_obj_align(label3, LV_ALIGN_CENTER, 0, -4);

    lv_group_add_obj(g, imgbtn1);
    lv_group_add_obj(g, imgbtn2);
    lv_group_add_obj(g, imgbtn3);
} 
