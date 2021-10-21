#include "lv_app.h"
#include "imgbtn_left.h"
#include "imgbtn_mid.h"
#include "imgbtn_right.h"

// static void event_handler(lv_event_t * e)
// {
//     lv_event_code_t code = lv_event_get_code(e);
//     lv_obj_t * obj = lv_event_get_target(e);
//     if(code == LV_EVENT_VALUE_CHANGED) {
//         printf("%d\n",lv_roller_get_selected(obj));
//     }
// }

static void mask_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    static int16_t mask_top_id = -1;
    static int16_t mask_bottom_id = -1;

    if (code == LV_EVENT_COVER_CHECK) {
        lv_event_set_cover_res(e, LV_COVER_RES_MASKED);

    } else if (code == LV_EVENT_DRAW_MAIN_BEGIN) {
        /* add mask */
        const lv_font_t * font = lv_obj_get_style_text_font(obj, LV_PART_MAIN);
        lv_coord_t line_space = lv_obj_get_style_text_line_space(obj, LV_PART_MAIN);
        lv_coord_t font_h = lv_font_get_line_height(font);

        lv_area_t roller_coords;
        lv_obj_get_coords(obj, &roller_coords);

        lv_area_t rect_area;
        rect_area.x1 = roller_coords.x1;
        rect_area.x2 = roller_coords.x2;
        rect_area.y1 = roller_coords.y1;
        rect_area.y2 = roller_coords.y1 + (lv_obj_get_height(obj) - font_h - line_space) / 2;

        lv_draw_mask_fade_param_t * fade_mask_top = lv_mem_buf_get(sizeof(lv_draw_mask_fade_param_t));
        lv_draw_mask_fade_init(fade_mask_top, &rect_area, LV_OPA_TRANSP, rect_area.y1, LV_OPA_COVER, rect_area.y2);
        mask_top_id = lv_draw_mask_add(fade_mask_top, NULL);

        rect_area.y1 = rect_area.y2 + font_h + line_space - 1;
        rect_area.y2 = roller_coords.y2;

        lv_draw_mask_fade_param_t * fade_mask_bottom =lv_mem_buf_get(sizeof(lv_draw_mask_fade_param_t));
        lv_draw_mask_fade_init(fade_mask_bottom, &rect_area, LV_OPA_COVER, rect_area.y1, LV_OPA_TRANSP, rect_area.y2);
        mask_bottom_id = lv_draw_mask_add(fade_mask_bottom, NULL);

    } else if (code == LV_EVENT_DRAW_POST_END) {
        lv_draw_mask_fade_param_t * fade_mask_top = lv_draw_mask_remove_id(mask_top_id);
        lv_draw_mask_fade_param_t * fade_mask_bottom = lv_draw_mask_remove_id(mask_bottom_id);
        lv_draw_mask_free_param(fade_mask_top);
        lv_draw_mask_free_param(fade_mask_bottom);
        lv_mem_buf_release(fade_mask_top);
        lv_mem_buf_release(fade_mask_bottom);
        mask_top_id = -1;
        mask_bottom_id = -1;
    }
}

void lv_app(lv_group_t * g)
{
    // LV_IMG_DECLARE(imgbtn_left);
    // LV_IMG_DECLARE(imgbtn_right);
    // LV_IMG_DECLARE(imgbtn_mid);

    // /*Create a transition animation on width transformation and recolor.*/
    // static lv_style_prop_t tr_prop[] = {LV_STYLE_TRANSFORM_WIDTH, LV_STYLE_IMG_RECOLOR_OPA, 0};
    // static lv_style_transition_dsc_t tr;
    // lv_style_transition_dsc_init(&tr, tr_prop, lv_anim_path_linear, 150, 0, NULL);

    // static lv_style_t style_def;
    // lv_style_init(&style_def);
    // lv_style_set_text_color(&style_def, lv_color_white());
    // lv_style_set_transition(&style_def, &tr);

    // /*Darken the button when pressed and make it wider*/
    // static lv_style_t style_pr;
    // lv_style_init(&style_pr);
    // lv_style_set_img_recolor_opa(&style_pr, LV_OPA_30);
    // lv_style_set_img_recolor(&style_pr, lv_color_black());
    // lv_style_set_transform_width(&style_pr, 10);

    // /*Create an image button*/
    // lv_obj_t * imgbtn1 = lv_imgbtn_create(lv_scr_act());
    // lv_imgbtn_set_src(imgbtn1, LV_IMGBTN_STATE_RELEASED, &imgbtn_left, &imgbtn_mid, &imgbtn_right);
    // lv_obj_add_style(imgbtn1, &style_def, 0);
    // lv_obj_add_style(imgbtn1, &style_pr, LV_STATE_FOCUSED);
    // lv_obj_align(imgbtn1, LV_ALIGN_CENTER, 0, -100);

    // lv_obj_t * imgbtn2 = lv_imgbtn_create(lv_scr_act());
    // lv_imgbtn_set_src(imgbtn2, LV_IMGBTN_STATE_RELEASED, &imgbtn_left, &imgbtn_mid, &imgbtn_right);
    // lv_obj_add_style(imgbtn2, &style_def, 0);
    // lv_obj_add_style(imgbtn2, &style_pr, LV_STATE_FOCUSED);
    // lv_obj_align(imgbtn2, LV_ALIGN_CENTER, 0, 0);

    // lv_obj_t * imgbtn3 = lv_imgbtn_create(lv_scr_act());
    // lv_imgbtn_set_src(imgbtn3, LV_IMGBTN_STATE_RELEASED, &imgbtn_left, &imgbtn_mid, &imgbtn_right);
    // lv_obj_add_style(imgbtn3, &style_def, 0);
    // lv_obj_add_style(imgbtn3, &style_pr, LV_STATE_FOCUSED);
    // lv_obj_align(imgbtn3, LV_ALIGN_CENTER, 0, 100);

    // /*Create a label on the image button*/
    // lv_obj_t * label1 = lv_label_create(imgbtn1);
    // lv_label_set_text(label1, "Click me <3");
    // lv_obj_align(label1, LV_ALIGN_CENTER, 0, -4);

    // lv_obj_t * label2 = lv_label_create(imgbtn2);
    // lv_label_set_text(label2, "Click me <3");
    // lv_obj_align(label2, LV_ALIGN_CENTER, 0, -4);

    // lv_obj_t * label3 = lv_label_create(imgbtn3);
    // lv_label_set_text(label3, "Click me <3");
    // lv_obj_align(label3, LV_ALIGN_CENTER, 0, -4);

    // lv_group_add_obj(g, imgbtn1);
    // lv_group_add_obj(g, imgbtn2);
    // lv_group_add_obj(g, imgbtn3);

    // lv_obj_t *roller1 = lv_roller_create(lv_scr_act());
    // lv_roller_set_options(roller1,
    //                     "January\n"
    //                     "February\n"
    //                     "March\n"
    //                     "April\n"
    //                     "May\n"
    //                     "June\n"
    //                     "July\n"
    //                     "August\n"
    //                     "September\n"
    //                     "October\n"
    //                     "November\n"
    //                     "December",
    //                     LV_ROLLER_MODE_INFINITE);

    // lv_roller_set_visible_row_count(roller1, 4);
    // lv_obj_center(roller1);
    // lv_obj_add_event_cb(roller1, event_handler, LV_EVENT_ALL, NULL);
    // lv_group_add_obj(g, roller1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_black());
    lv_style_set_text_color(&style, lv_color_white());
    lv_style_set_border_width(&style, 0);
    lv_style_set_pad_all(&style, 0);
    lv_obj_add_style(lv_scr_act(), &style, 0);

    lv_obj_t *roller1 = lv_roller_create(lv_scr_act());
    lv_obj_add_style(roller1, &style, 0);
    lv_obj_set_style_bg_opa(roller1, LV_OPA_TRANSP, LV_PART_SELECTED);

#if LV_FONT_MONTSERRAT_22
    lv_obj_set_style_text_font(roller1, &lv_font_montserrat_22, LV_PART_SELECTED);
#endif

    lv_roller_set_options(roller1,
                        "January\n"
                        "February\n"
                        "March\n"
                        "April\n"
                        "May\n"
                        "June\n"
                        "July\n"
                        "August\n"
                        "September\n"
                        "October\n"
                        "November\n"
                        "December",
                        LV_ROLLER_MODE_INFINITE);

    lv_obj_center(roller1);
    lv_roller_set_visible_row_count(roller1, 3);
    lv_obj_add_event_cb(roller1, mask_event_cb, LV_EVENT_ALL, NULL);
    lv_group_add_obj(g, roller1);
}