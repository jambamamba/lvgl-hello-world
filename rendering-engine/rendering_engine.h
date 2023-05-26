#pragma once
#include <stdint.h>
#include <lvgl/src/hal/lv_hal_indev.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef bool (*KeypadEventCallback)(uint32_t key, uint32_t btn_id);
typedef bool (*TouchEventCallback)(lv_point_t point);
typedef void (*WindowCloseEvent)();
void initRenderingEngine(KeypadEventCallback keypress_cb, TouchEventCallback pointer_cb, WindowCloseEvent window_close_event);
void deInitRenderingEngine();
void defaultMouseReadHandler(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
void defaultKeyboardReadOverride(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);

#ifdef __cplusplus
}
#endif
