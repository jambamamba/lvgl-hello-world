#include <rendering_engine.h>

#include <lv_drivers/display/fbdev.h>
#include <rendering_engine.h>
#include <stdio.h>
#include <stdlib.h>

#include <debug_logger.h>

LOG_CATEGORY(RENDERING_ENGINE, "RENDERING_ENGINE")
static void 
framebuffer_init()
{
  int res;
  res = system("/bin/systemctl stop system-weston.slice");                                                                                                                                  
  res = system("/bin/systemctl stop weston@root.service");  
  (void)res;
    //res = system("/usr/bin/pkill weston");
    //res = system("/bin/systemctl stop weston");
    fbdev_init();

    //osm todo: read from file SCREEN_HOR_RES SCREEN_VER_RES
    #define SCREEN_HOR_RES 800
    #define SCREEN_VER_RES 480
    /*A small buffer for LittlevGL to draw the screen's content*/
#define DISP_BUF_SIZE (SCREEN_HOR_RES * SCREEN_VER_RES * sizeof(uint32_t) * 100)
    static lv_color_t buf[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);
}

// void init_rendering_engine(const char *compositor)
void
initRenderingEngine(KeypadEventCallback keypress_cb, TouchEventCallback pointer_cb, WindowCloseEvent window_event_cb)
{
  lv_init();
  framebuffer_init();
}

void 
deInitRenderingEngine(){
  lv_deinit();
}