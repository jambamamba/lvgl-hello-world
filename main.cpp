#include <lvgl/demos/lv_demos.h>
#include <lvgl/examples/lv_examples.h>
#include <lvgl/lvgl.h>
#include <unistd.h>

#include "rendering-engine/rendering_engine.h"

static bool 
keypressEvent(uint32_t key, uint32_t btn_id){
    return true;
}

static bool 
touchEvent(lv_point_t point){
  return true;
}

static void 
screenEventCb(lv_event_t * e){ //osm todo: this cb is not working
//   LOG(DEBUG, MAIN, "@@@@@@@@@@ my_event_cb @@@@@@@@@@@ %i\n", (int)e->code);  
    // switch(event) {
    // }
}

static void 
onCloseWindow(){
}

static void 
addTextBox()
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_radius(&style, 5);

    lv_style_set_width(&style, 500);
    lv_style_set_height(&style, LV_SIZE_CONTENT);

    lv_style_set_pad_ver(&style, 20);
    lv_style_set_pad_left(&style, 5);

    lv_style_set_x(&style, lv_pct(10));
    lv_style_set_y(&style, 70);

    /*Create an object with the new style*/
    lv_obj_t * obj = lv_obj_create(lv_scr_act());
    lv_obj_add_style(obj, &style, 0);

    static lv_obj_t * _label_serialnum;
    _label_serialnum = lv_label_create(obj);
    lv_label_set_text(_label_serialnum, "Serial#");
}


static void 
taEventCallback(lv_event_t * e){ 
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * _ta = (lv_obj_t *)e->user_data;

    if(code == LV_EVENT_CLICKED 
      && lv_event_get_target(e)==_ta){
        const char * text = lv_textarea_get_text(_ta);
    }
}
static void 
addTextArea()
{
    static lv_obj_t * _ta = lv_textarea_create(lv_scr_act());
    lv_textarea_set_text(_ta, "");
    lv_obj_align(_ta, LV_ALIGN_TOP_MID, 0, 78);
    lv_obj_set_size(_ta, 330, 42);
    lv_textarea_set_max_length(_ta, 15);
    lv_textarea_set_text_selection(_ta, true);
    lv_textarea_set_one_line(_ta, true);
    lv_obj_add_event_cb(_ta, taEventCallback, LV_EVENT_CLICKED, _ta);//also triggered when Enter key is pressed
}

static void 
addStatusMessage()
{
    static lv_obj_t * _label_status = lv_label_create(lv_scr_act());
    lv_label_set_text(_label_status, "");
    lv_obj_align(_label_status, LV_ALIGN_TOP_MID, 0, 148);
    lv_obj_set_size(_label_status, 330, 42);
}

static void 
addLoaderArc()
{
  static lv_obj_t * _arc = lv_arc_create(lv_scr_act());
  lv_arc_set_bg_angles(_arc, 0, 360);
  lv_arc_set_angles(_arc, 270, 270);
  lv_obj_align(_arc, LV_ALIGN_CENTER, 0, 0);
  // lv_obj_set_size(_arc, 0, 0);
}

static void 
addProgressBar()
{
  static lv_obj_t * _progress_bar = lv_bar_create(lv_scr_act());
  lv_obj_set_size(_progress_bar, 200, 20);
  lv_obj_align(_progress_bar, LV_ALIGN_CENTER, 0, 0);
  // lv_bar_set_anim_time(_progress_bar, 2000);
  lv_bar_set_value(_progress_bar, 0, LV_ANIM_ON);
}

static void 
eventLoop(int loop_count)
{
  for (int i = 0; (loop_count < 0 || i < loop_count); ++i){
    lv_timer_handler();
    usleep(10*1000);
  }
}

int 
main(int argc, char** argv)
{
    initRenderingEngine(keypressEvent, touchEvent, onCloseWindow);
    lv_obj_t *screen = lv_obj_create(nullptr);
    lv_obj_add_event_cb(screen, screenEventCb, LV_EVENT_ALL, nullptr);
    addTextBox();
    addTextArea();
    addStatusMessage();
    addLoaderArc();
    addProgressBar();
    eventLoop(-1);
    return 0;
}