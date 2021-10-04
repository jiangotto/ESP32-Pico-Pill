#include "Button.h"

int state_last[4] = {1};
int state_pr[4] = {1};
int lv_key;

void Button_init(void)
{
    //back
    gpio_pad_select_gpio(PIN_BACK);
    gpio_set_direction(PIN_BACK, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PIN_BACK, GPIO_PULLUP_ONLY);
    //up
    gpio_pad_select_gpio(PIN_UP);
    gpio_set_direction(PIN_UP, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PIN_UP, GPIO_PULLUP_ONLY);
    //select
    gpio_pad_select_gpio(PIN_SELECT);
    gpio_set_direction(PIN_SELECT, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PIN_SELECT, GPIO_PULLUP_ONLY);
    //down
    gpio_pad_select_gpio(PIN_DOWN);
    gpio_set_direction(PIN_DOWN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PIN_DOWN, GPIO_PULLUP_ONLY);
}

int Button_lvgl_cb(void)
{
    state_pr[0] = gpio_get_level(PIN_BACK);
    state_pr[1] = gpio_get_level(PIN_UP);
    state_pr[2] = gpio_get_level(PIN_SELECT);
    state_pr[3] = gpio_get_level(PIN_DOWN);

    if(state_pr[0] == 0 && state_last[0] == 1)
        lv_key = LV_KEY_ESC_t;
    else if(state_pr[1] == 0 && state_last[1] == 1)
        lv_key = LV_KEY_LEFT_t;
    else if(state_pr[2] == 0 && state_last[2] == 1)
        lv_key = LV_KEY_ENTER_t;
    else if(state_pr[3] == 0 && state_last[3] == 1)
        lv_key = LV_KEY_RIGHT_t;
    else 
        lv_key = -1;

    state_last[0] = state_pr[0];
    state_last[1] = state_pr[1];
    state_last[2] = state_pr[2];
    state_last[3] = state_pr[3];

    return lv_key;
}