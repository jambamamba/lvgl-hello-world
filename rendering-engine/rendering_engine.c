#include "rendering_engine.h"

#include <lv_drivers/sdl/sdl.h>
#include <SDL2/SDL.h>
#include <sys/time.h>

#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
#define SCREEN_HOR_RES MONITOR_HOR_RES
#define SCREEN_VER_RES MONITOR_VER_RES

static KeypadEventCallback _keypress_callback = 0;
static void 
sdlKeyboardReadOverride(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
  sdl_keyboard_read(indev_drv, data);
  if(data->state == LV_INDEV_STATE_PRESSED){
    if(_keypress_callback){
      _keypress_callback(data->key, data->btn_id);
    }
  }
}

uint32_t 
custom_tick_get(void)
{
  static uint64_t start_ms = 0;
  if (start_ms == 0)
  {
    struct timeval tv_start;
    gettimeofday(&tv_start, NULL);
    start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
  }

  struct timeval tv_now;
  gettimeofday(&tv_now, NULL);
  uint64_t now_ms;
  now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

  uint32_t time_ms = now_ms - start_ms;
  return time_ms;
}

/**
 * A task to measure the elapsed time for LVGL
 * @param data unused
 * @return never return
 */
static int 
tickThread(void *data) {
    (void)data;

    while(1) { 
        SDL_Delay(5);
        // lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
    }

    return 0;
}


typedef void (*WindowEventCallback)(struct SDL_WindowEvent *e);
/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
lv_disp_t *_display;
lv_disp_drv_t _display_driver;
void 
initRenderingEngineSDL(KeypadEventCallback keypress_cb, WindowEventCallback window_event_cb)
{
  _keypress_callback = keypress_cb;
  /* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
  // monitor_init();
  lv_init();
  sdl_init(window_event_cb);
  // SDL_ShowCursor(false);
  /* Tick init.
   * You have to call 'lv_tick_inc()' in periodically to inform LittelvGL about
   * how much time were elapsed Create an SDL thread to do this*/
  // SDL_CreateThread(tickThread, "tick", NULL);

  /*Create a display buffer*/
  static lv_disp_draw_buf_t disp_buf1 = {0};
  static lv_color_t buf1_1[SCREEN_HOR_RES * SCREEN_VER_RES * 3] = {0};
  static lv_color_t buf1_2[SCREEN_HOR_RES * SCREEN_VER_RES * 3] = {0};
  lv_disp_draw_buf_init(&disp_buf1, buf1_1, buf1_2, SCREEN_HOR_RES * SCREEN_VER_RES);

  /*Create a display*/
  lv_disp_drv_init(&_display_driver); /*Basic initialization*/
  _display_driver.draw_buf = &disp_buf1;
  _display_driver.flush_cb = sdl_display_flush;//monitor_flush;
  _display_driver.hor_res = SCREEN_HOR_RES;
  _display_driver.ver_res = SCREEN_VER_RES;
  _display_driver.antialiasing = 1;
  _display = lv_disp_drv_register(&_display_driver);

  lv_theme_t * th = lv_theme_default_init(_display, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
  lv_disp_set_theme(_display, th);

  lv_group_t * g = lv_group_create();
  lv_group_set_default(g);

  // sdl_keyboard_init();
  static lv_indev_drv_t indev_drv_2;
  lv_indev_drv_init(&indev_drv_2); /*Basic initialization*/
  indev_drv_2.type = LV_INDEV_TYPE_KEYPAD;
  indev_drv_2.read_cb = sdlKeyboardReadOverride;//sdl_keyboard_read;
  lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv_2);
  lv_indev_set_group(kb_indev, g);
  // sdl_mousewheel_init();
  static lv_indev_drv_t indev_drv_3;
  lv_indev_drv_init(&indev_drv_3); /*Basic initialization*/
  indev_drv_3.type = LV_INDEV_TYPE_ENCODER;
  indev_drv_3.read_cb = sdl_mousewheel_read;

  lv_indev_t * enc_indev = lv_indev_drv_register(&indev_drv_3);
  lv_indev_set_group(enc_indev, g);

#if 1
  /* Add the mouse as input device
   * Use the 'mouse' driver which reads the PC's mouse*/
  // sdl_mouse_init();
  static lv_indev_drv_t indev_drv_1;
  lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
  indev_drv_1.type = LV_INDEV_TYPE_POINTER;

  /*This function will be called periodically (by the library) to get the mouse position and state*/
  indev_drv_1.read_cb = sdl_mouse_read;
  lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);
#endif
#if 0
  /*Set a cursor for the mouse*/
  LV_IMG_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
  lv_obj_t * cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor */
  lv_img_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
  lv_indev_set_cursor(mouse_indev, cursor_obj);             /*Connect the image  object to the driver*/
#endif
}
