#include "appmessage.h"

static Window *schedulesWindow;
static MenuLayer *schedulesMenuLayer;

static char *direction;
static char *line;

static Schedule schedules[4];
static int schedulesIndex;

// menu layer
static uint16_t schedules_menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return ARRAY_LENGTH(schedules);
}

static uint16_t schedules_menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return 1;
}

static int16_t schedules_menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void schedules_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  menu_cell_basic_header_draw(ctx, cell_layer, schedules[section_index].name);
}

static void schedules_menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  menu_cell_title_draw(ctx, cell_layer, schedules[cell_index->section].arriving);
}

static void schedules_menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Select %i", cell_index->row);
}

// window
static void schedules_window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "schedules window for line %s direction %s", line, direction);

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  schedulesMenuLayer = menu_layer_create(bounds);

  menu_layer_set_callbacks(schedulesMenuLayer, NULL, (MenuLayerCallbacks){
    .get_num_sections = schedules_menu_get_num_sections_callback,
    .get_num_rows = schedules_menu_get_num_rows_callback,
    .get_header_height = schedules_menu_get_header_height_callback,
    .draw_header = schedules_menu_draw_header_callback,
    .draw_row = schedules_menu_draw_row_callback,
    .select_click = schedules_menu_select_callback,
  });

  menu_layer_set_click_config_onto_window(schedulesMenuLayer, window);

  layer_add_child(window_layer, menu_layer_get_layer(schedulesMenuLayer));

  schedulesIndex = 0;

  DictionaryIterator *outbox;
  app_message_outbox_begin(&outbox);
  dict_write_cstring(outbox, SELECTED_DIRECTION_KEY, direction);
  dict_write_cstring(outbox, SELECTED_LINE_KEY, line);
  app_message_outbox_send();
}

static void schedules_window_unload(Window *window) {
  window_destroy(window);
}

// init
Window *init_schedules_window(char *lineKey, char *directionKey) {
  line = lineKey;
  direction = directionKey;
  schedulesWindow = window_create();
  window_set_window_handlers(schedulesWindow, (WindowHandlers) {
    .load = schedules_window_load,
    .unload = schedules_window_unload,
  });
  return schedulesWindow;
}