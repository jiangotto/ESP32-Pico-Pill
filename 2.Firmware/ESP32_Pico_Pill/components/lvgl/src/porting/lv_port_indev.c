/**
 * @file lv_port_indev_templ.c
 *
 */

 /*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void encoder_init(void);
static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);


/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t * indev_keypad;
lv_group_t * g;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_indev_init(void)
{
    /**
     * Here you will find example implementation of input devices supported by LittelvGL:
     *  - Touchpad
     *  - Mouse (with cursor support)
     *  - Keypad (supports GUI usage only with key)
     *  - Encoder (supports GUI usage only with: left, right, push)
     *  - Button (external buttons to press points on the screen)
     *
     *  The `..._read()` function are only examples.
     *  You should shape them according to your hardware
     */

    static lv_indev_drv_t indev_drv;

    /*------------------
     * Encoder
     * -----------------*/

    /*Initialize your encoder if you have*/
    encoder_init();

    /*Register a encoder input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = keypad_read;
    indev_keypad = lv_indev_drv_register(&indev_drv);

    /*Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
     *add objects to the group with `lv_group_add_obj(group, obj)`
     *and assign this input device to group to navigate in it:
     *`lv_indev_set_group(indev_encoder, group);`*/
    g = lv_group_create();
    //lv_group_set_default(g);
    lv_indev_set_group(indev_keypad, g);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * Encoder
 * -----------------*/

/*Initialize your keypad*/
static void encoder_init(void)
{
    /*Your code comes here*/
    Button_init();
}

/*Will be called by the library to read the encoder*/
static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    // data->key = Button_lvgl_cb();    /*Get the ID (0,1,2...) of the pressed button*/
    // if(data->key == LV_KEY_ENTER)
    //     data->state = LV_INDEV_STATE_PR; 
    // else if(data->key == LV_KEY_RIGHT)
    // {
    //     data->enc_diff = 1;
    //     data->state = LV_INDEV_STATE_REL;
    // }
    // else if(data->key == LV_KEY_LEFT)
    // {
    //     data->enc_diff = -1;
    //     data->state = LV_INDEV_STATE_REL;
    // }

    uint32_t act_key = Button_lvgl_cb();
    if(act_key != -1)
    {
        data->state = LV_INDEV_STATE_PR;
        data->key = act_key;
    }
    else
        data->state = LV_INDEV_STATE_REL;
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
