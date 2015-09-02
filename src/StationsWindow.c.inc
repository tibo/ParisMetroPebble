#include "DestinationsWindow.c.inc"

static Window *stationsWindow;
static MenuLayer *stationsMenuLayer;

static char stations[5][128]; 
static int stationsIndex = 0;

// menu layer
static uint16_t stations_menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return 1;
}

static uint16_t stations_menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return ARRAY_LENGTH(stations);
}

static int16_t stations_menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void stations_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  menu_cell_basic_header_draw(ctx, cell_layer, "Stations");
}

static void stations_menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  menu_cell_title_draw(ctx, cell_layer, stations[cell_index->row]);
}

static void stations_menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  // APP_LOG(APP_LOG_LEVEL_DEBUG, "Select %i", cell_index->row);
  Window *destinationsWindow = init_destinations_window(stations[cell_index->row]);
  window_stack_push(destinationsWindow, true);
}

// window
static void stations_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  stationsMenuLayer = menu_layer_create(bounds);

  menu_layer_set_callbacks(stationsMenuLayer, NULL, (MenuLayerCallbacks){
    .get_num_sections = stations_menu_get_num_sections_callback,
    .get_num_rows = stations_menu_get_num_rows_callback,
    .get_header_height = stations_menu_get_header_height_callback,
    .draw_header = stations_menu_draw_header_callback,
    .draw_row = stations_menu_draw_row_callback,
    .select_click = stations_menu_select_callback,
  });

  menu_layer_set_click_config_onto_window(stationsMenuLayer, window);

  layer_add_child(window_layer, menu_layer_get_layer(stationsMenuLayer));
}

static void stations_window_unload(Window *window) {
  menu_layer_destroy(stationsMenuLayer);
  window_destroy(window);
}

// init
Window *init_station_window() {
  stationsWindow = window_create();
  window_set_window_handlers(stationsWindow, (WindowHandlers) {
    .load = stations_window_load,
    .unload = stations_window_unload,
  });
  return stationsWindow;
}