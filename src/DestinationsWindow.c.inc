
static Window *destinationsWindow;
static MenuLayer *destinationsMenuLayer;

static char *selectedStationKey;

// menu layer
static uint16_t destinations_menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return 1;
}

static uint16_t destinations_menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return 5;
}

static int16_t destinations_menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void destinations_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  menu_cell_basic_header_draw(ctx, cell_layer, selectedStationKey);
}

static void destinations_menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  menu_cell_title_draw(ctx, cell_layer, "Test");
}

static void destinations_menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Select %i", cell_index->row);
}

// window
static void destinations_window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "load destination window %p for station %s",window, selectedStationKey);

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  destinationsMenuLayer = menu_layer_create(bounds);

  menu_layer_set_callbacks(destinationsMenuLayer, NULL, (MenuLayerCallbacks){
    .get_num_sections = destinations_menu_get_num_sections_callback,
    .get_num_rows = destinations_menu_get_num_rows_callback,
    .get_header_height = destinations_menu_get_header_height_callback,
    .draw_header = destinations_menu_draw_header_callback,
    .draw_row = destinations_menu_draw_row_callback,
    .select_click = destinations_menu_select_callback,
  });

  menu_layer_set_click_config_onto_window(destinationsMenuLayer, window);

  layer_add_child(window_layer, menu_layer_get_layer(destinationsMenuLayer));
}

static void destinations_window_unload(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "unload test window %p",window);
  menu_layer_destroy(destinationsMenuLayer);
  window_destroy(window);
}

// init
Window *init_destinations_window(char *station) {
  selectedStationKey = station;
  destinationsWindow = window_create();
  window_set_window_handlers(destinationsWindow, (WindowHandlers) {
    .load = destinations_window_load,
    .unload = destinations_window_unload,
  });
  return destinationsWindow;
}