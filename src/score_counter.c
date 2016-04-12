#include "pebble.h"

#define NUM_MENU_SECTIONS 2
#define NUM_FIRST_MENU_ITEMS 3
#define NUM_SECOND_MENU_ITEMS 1

static Window *s_main_window;
static SimpleMenuLayer *s_simple_menu_layer;
static SimpleMenuSection s_menu_sections[NUM_MENU_SECTIONS];
static SimpleMenuItem s_first_menu_items[NUM_FIRST_MENU_ITEMS];
static int s_step_count = 0, s_current_score = 0;
static bool fin = false;


/*static void open_new_window(int index, void *ctx){
  new_window_push();
}*/
static void get_step_count() {
  s_step_count = (int)health_service_sum_today(HealthMetricStepCount);
}

static void menu_select_callback(int index, void *ctx) {
  static char c_output[9];
  if(!fin){
    get_step_count();
    s_current_score = s_step_count;
    snprintf(c_output, sizeof(c_output), "%d", s_current_score);
    s_first_menu_items[index].subtitle =  c_output;
    fin = true;
  }else{
    get_step_count();
    s_current_score = s_step_count - s_current_score;
    snprintf(c_output, sizeof(c_output), "fin %d", s_current_score);
    s_first_menu_items[index].subtitle =  c_output;
    fin = false;
  }

  layer_mark_dirty(simple_menu_layer_get_layer(s_simple_menu_layer));
}


static void main_window_load(Window *window) {

  // Although we already defined NUM_FIRST_MENU_ITEMS, you can define
  // an int as such to easily change the order of menu items later
  int num_a_items = 0;

  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Cycling",
    .callback = menu_select_callback,
  };
  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Running",
    .callback = menu_select_callback,
  };
  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Swiming",
    .callback = menu_select_callback,
  };
  /*
  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Third Item",
    .subtitle = PBL_IF_RECT_ELSE("This has an icon", "Item number three"),
    .callback = open_new_window,
    .icon = PBL_IF_RECT_ELSE(s_menu_icon_image, NULL),
  };*/

  s_menu_sections[0] = (SimpleMenuSection) {
    .num_items = NUM_FIRST_MENU_ITEMS,
    .items = s_first_menu_items,
  };

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_simple_menu_layer = simple_menu_layer_create(bounds, window, s_menu_sections, NUM_MENU_SECTIONS, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer));
}

void main_window_unload(Window *window) {
  simple_menu_layer_destroy(s_simple_menu_layer);
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
