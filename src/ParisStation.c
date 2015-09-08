#include <pebble.h>
#include "StationsWindow.c.inc"
#include "appmessage.h"

// app message
void message_received(DictionaryIterator *iter, void *context) {
  Tuple *tuple = dict_read_first(iter);
  while (tuple) {
    switch(tuple->key){
      case NEW_STATION_KEY: {
        if (strlen(tuple->value->cstring) > 0) {
          APP_LOG(APP_LOG_LEVEL_DEBUG, "new station key: %s", tuple->value->cstring);
          snprintf(stations[stationsIndex].key, sizeof(stations[stationsIndex]), tuple->value->cstring);
        }
        break;
      }
      case NEW_STATION_NAME: {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "new station name: %s", tuple->value->cstring);
        snprintf(stations[stationsIndex].name, sizeof(stations[stationsIndex]), tuple->value->cstring);
        stationsIndex++;
        break;
      }
      case END_STATIONS_KEY: {
        if (tuple->value->uint8 > 0) {
          APP_LOG(APP_LOG_LEVEL_DEBUG, "reloading menu layer");
          menu_layer_reload_data(stationsMenuLayer);
        }
        break;
      }
      case NEW_LINE_KEY : {
        if (strlen(tuple->value->cstring) > 0) {
          APP_LOG(APP_LOG_LEVEL_DEBUG, "new line: %s", tuple->value->cstring);
          snprintf(lines[linesIndex].name, sizeof(lines[linesIndex]), tuple->value->cstring);
          linesIndex++;
          directionsIndex = 0;
        }
        break;
      }
      case NEW_DESTINATION_NAME_KEY : {
        if (strlen(tuple->value->cstring) > 0) {
          APP_LOG(APP_LOG_LEVEL_DEBUG, "new destination name: %s", tuple->value->cstring);
          snprintf(lines[linesIndex-1].destinations[directionsIndex].name, sizeof(lines[linesIndex-1].destinations[directionsIndex]), tuple->value->cstring);
        }
        break;
      }
      case NEW_DESTINATION_DIRECTION_KEY : {
        if (strlen(tuple->value->cstring) > 0) {
          APP_LOG(APP_LOG_LEVEL_DEBUG, "new destination: %s", tuple->value->cstring);
          snprintf(lines[linesIndex-1].destinations[directionsIndex].direction, sizeof(lines[linesIndex-1].destinations[directionsIndex]), tuple->value->cstring);
          directionsIndex++;
        }
        break;
      }
      case END_DESTINATIONS_KEY : {
        if (tuple->value->uint8 > 0) {
          APP_LOG(APP_LOG_LEVEL_DEBUG, "reloading menu layer");
          menu_layer_reload_data(destinationsMenuLayer);
        }
        break;
      }
      default: {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Unkown key %d", (int)tuple->key);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "String value: %s", tuple->value->cstring);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Int value: %i", (int)tuple->value->uint8);
        break;
      }
    }

    tuple = dict_read_next(iter);
  }
}

// init/deinit
static void init(void) {
  Window *stationsWindow = init_station_window();
  window_stack_push(stationsWindow, true);

  app_message_register_inbox_received(message_received);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit(void) {
}

// main
int main(void) {
  init();
  app_event_loop();
  deinit();
}
