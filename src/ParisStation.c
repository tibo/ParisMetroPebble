#include <pebble.h>

static Window *stationsWindow;

static MenuLayer *stationsMenuLayer;

static AppSync s_sync;
static uint8_t s_sync_buffer[64];

enum NotificationsKey {
  NEW_STATION_KEY = 0x0,
  END_STATIONS_KEY = 0x1
};

char *translate_error(AppMessageResult result) {
  switch (result) {
    case APP_MSG_OK: return "APP_MSG_OK";
    case APP_MSG_SEND_TIMEOUT: return "APP_MSG_SEND_TIMEOUT";
    case APP_MSG_SEND_REJECTED: return "APP_MSG_SEND_REJECTED";
    case APP_MSG_NOT_CONNECTED: return "APP_MSG_NOT_CONNECTED";
    case APP_MSG_APP_NOT_RUNNING: return "APP_MSG_APP_NOT_RUNNING";
    case APP_MSG_INVALID_ARGS: return "APP_MSG_INVALID_ARGS";
    case APP_MSG_BUSY: return "APP_MSG_BUSY";
    case APP_MSG_BUFFER_OVERFLOW: return "APP_MSG_BUFFER_OVERFLOW";
    case APP_MSG_ALREADY_RELEASED: return "APP_MSG_ALREADY_RELEASED";
    case APP_MSG_CALLBACK_ALREADY_REGISTERED: return "APP_MSG_CALLBACK_ALREADY_REGISTERED";
    case APP_MSG_CALLBACK_NOT_REGISTERED: return "APP_MSG_CALLBACK_NOT_REGISTERED";
    case APP_MSG_OUT_OF_MEMORY: return "APP_MSG_OUT_OF_MEMORY";
    case APP_MSG_CLOSED: return "APP_MSG_CLOSED";
    case APP_MSG_INTERNAL_ERROR: return "APP_MSG_INTERNAL_ERROR";
    default: return "UNKNOWN ERROR";
  }
}

static char stations[5][128]; 
static int stationsIndex = 0;

// app sync
static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %s", translate_error(app_message_error));
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  switch(key) {
    case NEW_STATION_KEY: {
      if (strlen(new_tuple->value->cstring) > 0) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "new station key: %s", new_tuple->value->cstring);
        snprintf(stations[stationsIndex], sizeof(stations[stationsIndex]), new_tuple->value->cstring);
        stationsIndex++;
      }
      break;
    }
    case END_STATIONS_KEY: {
      if (new_tuple->value->uint8 > 0) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "reloading menu layer");
        menu_layer_reload_data(stationsMenuLayer);
      }
      break;
    }
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Unkown key %d", (int) key);
      break;
  }
}

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
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Select %i", cell_index->row);
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

  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  Tuplet initial_values[] = {
    TupletCString(NEW_STATION_KEY,""),
    TupletInteger(END_STATIONS_KEY, (uint8_t) 0)
  };

  app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer),
      initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL);
}

static void stations_window_unload(Window *window) {

  menu_layer_destroy(stationsMenuLayer);
}

// init/deinit
static void init(void) {
  stationsWindow = window_create();
  window_set_window_handlers(stationsWindow, (WindowHandlers) {
    .load = stations_window_load,
    .unload = stations_window_unload,
  });
  const bool animated = true;
  window_stack_push(stationsWindow, animated);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", stationsWindow);
}

static void deinit(void) {
  window_destroy(stationsWindow);
  app_sync_deinit(&s_sync);
}

// main
int main(void) {
  init();
  app_event_loop();
  deinit();
}
