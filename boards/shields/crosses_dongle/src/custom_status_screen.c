/*
 * Copyright (c) 2024 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 */

#include "custom_status_screen.h"

#if CONFIG_DONGLE_SCREEN_OUTPUT_ACTIVE
#include "widgets/output_status.h"
static struct zmk_widget_output_status output_status_widget;
#endif

#if CONFIG_DONGLE_SCREEN_LAYER_ACTIVE
#include "widgets/layer_status.h"
static struct zmk_widget_layer_status layer_status_widget;
#endif

#if CONFIG_DONGLE_SCREEN_BATTERY_ACTIVE
#include "widgets/battery_status.h"
static struct zmk_widget_dongle_battery_status dongle_battery_status_widget;
#endif

#if CONFIG_DONGLE_SCREEN_WPM_ACTIVE
#include "widgets/wpm_status.h"
static struct zmk_widget_wpm_status wpm_status_widget;
#endif

#if CONFIG_DONGLE_SCREEN_MODIFIER_ACTIVE
#include "widgets/mod_status.h"
static struct zmk_widget_mod_status mod_widget;
#endif

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

lv_style_t global_style;

static void bg_opacity_anim_cb(void *var, int32_t value)
{
    lv_obj_set_style_bg_opa(var, value, 0);
}

lv_obj_t *zmk_display_status_screen()
{
    lv_obj_t *screen;

    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, 255, LV_PART_MAIN);

    // Animated background layer
    lv_obj_t *bg_layer = lv_obj_create(screen);
    lv_obj_set_size(bg_layer, 240, 135);
    lv_obj_set_style_bg_color(bg_layer, lv_color_hex(0x001a4d), 0);
    lv_obj_set_style_bg_opa(bg_layer, 50, 0);
    lv_obj_clear_flag(bg_layer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(bg_layer, 0, 0);
    lv_obj_align(bg_layer, LV_ALIGN_CENTER, 0, 0);

    // Breathing animation
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, bg_layer);
    lv_anim_set_values(&a, 30, 80);
    lv_anim_set_time(&a, 4000);
    lv_anim_set_exec_cb(&a, bg_opacity_anim_cb);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_playback_time(&a, 4000);
    lv_anim_start(&a);

    lv_style_init(&global_style);
    lv_style_set_text_color(&global_style, lv_color_white());
    lv_style_set_text_letter_space(&global_style, 1);
    lv_style_set_text_line_space(&global_style, 1);
    lv_obj_add_style(screen, &global_style, LV_PART_MAIN);

#if CONFIG_DONGLE_SCREEN_OUTPUT_ACTIVE
    zmk_widget_output_status_init(&output_status_widget, screen);
    lv_obj_align(zmk_widget_output_status_obj(&output_status_widget), LV_ALIGN_TOP_MID, 0, 10);
#endif

#if CONFIG_DONGLE_SCREEN_BATTERY_ACTIVE
    zmk_widget_dongle_battery_status_init(&dongle_battery_status_widget, screen);
    lv_obj_align(zmk_widget_dongle_battery_status_obj(&dongle_battery_status_widget), LV_ALIGN_BOTTOM_MID, 0, 0);
#endif

#if CONFIG_DONGLE_SCREEN_WPM_ACTIVE
    zmk_widget_wpm_status_init(&wpm_status_widget, screen);
    lv_obj_align(zmk_widget_wpm_status_obj(&wpm_status_widget), LV_ALIGN_TOP_LEFT, 20, 20);
#endif

#if CONFIG_DONGLE_SCREEN_LAYER_ACTIVE
    zmk_widget_layer_status_init(&layer_status_widget, screen);
    lv_obj_align(zmk_widget_layer_status_obj(&layer_status_widget), LV_ALIGN_CENTER, 0, 0);
#endif

#if CONFIG_DONGLE_SCREEN_MODIFIER_ACTIVE
    zmk_widget_mod_status_init(&mod_widget, screen);
    lv_obj_align(zmk_widget_mod_status_obj(&mod_widget), LV_ALIGN_CENTER, 0, 35);
#endif

    return screen;
}
