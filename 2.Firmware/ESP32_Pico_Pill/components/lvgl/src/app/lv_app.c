#include "lv_app.h"
#include "imgbtn_left.h"
#include "imgbtn_mid.h"
#include "imgbtn_right.h"

void lv_app(lv_group_t * g)
{
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