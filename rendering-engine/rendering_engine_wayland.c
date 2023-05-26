#include <rendering_engine.h>

#include <lv_drivers/display/fbdev.h>
#include <lv_drivers/wayland/wayland.h>
#include <rendering_engine.h>
#include <stdio.h>
#include <stdlib.h>

// #include <debug_logger.h>

// LOG_CATEGORY(RENDERING_ENGINE, "RENDERING_ENGINE")


static TouchEventCallback _touch_callback;
void 
defaultMouseReadHandler(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
  if(!_touch_callback(data->point)){
    return;
  }
  //osm lv_wayland_touch_read(indev_drv, data);
}

static KeypadEventCallback _keypress_callback = 0;
void 
defaultKeyboardReadOverride(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
  if(data->state == LV_INDEV_STATE_PRESSED){
    if(!_keypress_callback(data->key, data->btn_id)){
      return;
    }
  }
  //osm lv_wayland_keyboard_read(indev_drv, data);
}


static WindowCloseEvent _window_event_cb;
// typedef bool (*lv_wayland_display_close_f_t)(lv_disp_t * disp);
bool lvWaylandDisplayClose(lv_disp_t * disp){
  _window_event_cb();
}

#if 0 //osm
    //osm todo: read from file SCREEN_HOR_RES SCREEN_VER_RES
    #define SCREEN_HOR_RES 800
    #define SCREEN_VER_RES 480
#define DISP_BUF_SIZE (80 * SCREEN_HOR_RES)
lv_disp_drv_t _display_driver;
lv_disp_t *_display;
void wayland_init1()
{
  //osm: might need to start these services if they were stopped, framebuffer_init() stops them
  // int res;
  // res = system("/bin/systemctl start system-weston.slice");                                                                                                                                  
  // res = system("/bin/systemctl start weston@root.service");  
  // usleep(1000 * 1000);//wait for weston to start - need a better way to do this
  lv_wayland_init();
  
    //from wayland example start
    static lv_disp_draw_buf_t disp_buf1;
    static lv_color_t buf1[DISP_BUF_SIZE * 100];
    lv_disp_draw_buf_init(&disp_buf1, buf1, NULL, DISP_BUF_SIZE * 100);

    /* Create a display */
    lv_disp_drv_init(&_display_driver);
    _display_driver.draw_buf = &disp_buf1;
    _display_driver.flush_cb = lv_wayland_flush;
    _display_driver.hor_res    = SCREEN_HOR_RES;
    _display_driver.ver_res    = SCREEN_VER_RES;
    _display = lv_disp_drv_register(&_display_driver);
    
    static lv_indev_drv_t indev_drv_touch;
    lv_indev_drv_init(&indev_drv_touch);
    indev_drv_touch.type = LV_INDEV_TYPE_POINTER;
    extern void mouseReadOverride(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
    indev_drv_touch.read_cb = mouseReadOverride;//wayland_touch_read;
    lv_indev_drv_register(&indev_drv_touch);
}
#endif

//osm todo: read from file SCREEN_HOR_RES SCREEN_VER_RES
// #define SCREEN_HOR_RES 800
// #define SCREEN_VER_RES 480
// void init_rendering_engine(const char *compositor)
void
initRenderingEngine(KeypadEventCallback keypress_cb, TouchEventCallback pointer_cb, WindowCloseEvent window_event_cb){
  _keypress_callback = keypress_cb;
  _touch_callback = pointer_cb;
  _window_event_cb = window_event_cb;
  lv_init();
  lv_wayland_init();
  static lv_disp_t *display;
  static lv_disp_drv_t display_driver;
  display = lv_wayland_create_window(SCREEN_HOR_RES*.8, SCREEN_VER_RES*.8, //osm fix this
    "Window Title", lvWaylandDisplayClose);
  // lv_wayland_window_set_fullscreen(display, false);//osm fix this: full screen true crashes!
  //set env var LV_WAYLAND_DISABLE_WINDOWDECORATION=1 

  static lv_indev_drv_t indev_drv_keyboard;
  lv_indev_drv_init(&indev_drv_keyboard); /*Basic initialization*/
  indev_drv_keyboard.type = LV_INDEV_TYPE_KEYPAD;
  extern void keyboardReadOverride(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
  indev_drv_keyboard.read_cb = defaultKeyboardReadOverride;//keyboardReadOverride
  lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv_keyboard);

  static lv_indev_drv_t indev_drv_touch;
  lv_indev_drv_init(&indev_drv_touch);
  indev_drv_touch.type = LV_INDEV_TYPE_POINTER;
  extern void mouseReadOverride(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
  indev_drv_touch.read_cb = defaultMouseReadHandler;//mouseReadOverride;
  lv_indev_drv_register(&indev_drv_touch);

//   if(compositor && *compositor)
//   {
//     if(strcmp(compositor, "wayland") == 0)
//     {
//       wayland_init1();
//     }
//     else if(strcmp(compositor, "framebuffer") == 0)
//     {
//       framebuffer_init();
//     }
//     else
//     {
//       LOG(WARNING, RENDERING_ENGINE, "unrecognized rendering engine '%s'\nEither use wayland or framebuffer as second parameter.", compositor);
//       return;
//     }
//   }
//   else
//   {
//     LOG(DEBUG, RENDERING_ENGINE, "switching to default rendering engine 'wayland'\n");
//     wayland_init1();
//   }
}

void 
deInitRenderingEngine(){
  lv_deinit();
  lv_wayland_deinit();
}