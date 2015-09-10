#include <pebble.h>
#include "StationsWindow.c.inc"
#include "appmessage.h"

// app message
void message_received(DictionaryIterator *iter, void *context) {
  Tuple *tuple = dict_read_first(iter);
  while (tuple) {
    switch(tuple->key){
      case NEW_STATION_KEY: {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "new station key: %s", tuple->value->cstring);
        stations[stationsIndex].key = malloc(sizeof(char [strlen(tuple->value->cstring)+1]));
        strcpy(stations[stationsIndex].key, tuple->value->cstring);
        break;
      }
      case NEW_STATION_NAME: {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "new station name: %s", tuple->value->cstring);
        stations[stationsIndex].name = malloc(sizeof(char [strlen(tuple->value->cstring)+1]));
        strcpy(stations[stationsIndex].name, tuple->value->cstring);
        stationsIndex++;
        break;
      }
      case END_STATIONS_KEY: {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "reloading stations menu layer");
        menu_layer_reload_data(stationsMenuLayer);
        break;
      }
      case NEW_LINE_KEY : {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "new line: %s length: %i %i", tuple->value->cstring, strlen(tuple->value->cstring), sizeof(char [strlen(tuple->value->cstring)]));
        lines[linesIndex].name = malloc(sizeof(char [strlen(tuple->value->cstring)+1]));
        strcpy(lines[linesIndex].name, tuple->value->cstring);
        linesIndex++;
        directionsIndex = 0;
        break;
      }
      case NEW_DESTINATION_NAME_KEY : {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "new destination name: %s length: %i %i", tuple->value->cstring, strlen(tuple->value->cstring), sizeof(char [strlen(tuple->value->cstring)]));
        lines[linesIndex-1].destinations[directionsIndex].name = malloc(sizeof(char [strlen(tuple->value->cstring)+1]));
        strcpy(lines[linesIndex-1].destinations[directionsIndex].name, tuple->value->cstring);
        break;
      }
      case NEW_DESTINATION_DIRECTION_KEY : {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "new destination: %s", tuple->value->cstring);
        lines[linesIndex-1].destinations[directionsIndex].direction = malloc(sizeof(char [strlen(tuple->value->cstring)+1]));
        strcpy(lines[linesIndex-1].destinations[directionsIndex].direction, tuple->value->cstring);
        directionsIndex++;
        break;
      }
      case END_DESTINATIONS_KEY : {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "reloading destinations menu layer");
        menu_layer_reload_data(destinationsMenuLayer);
        break;
      }
      case NEW_SCHEDULE_DESTINATION_KEY : {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "new schedule destination: %s", tuple->value->cstring);
        schedules[schedulesIndex].name = malloc(sizeof(char [strlen(tuple->value->cstring)+1]));
        strcpy(schedules[schedulesIndex].name, tuple->value->cstring);
        break;
      }
      case NEW_SCHEDULE_TIME_KEY : {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "new schedule time: %s", tuple->value->cstring);
        schedules[schedulesIndex].arriving = malloc(sizeof(char [strlen(tuple->value->cstring)+1]));
        strcpy(schedules[schedulesIndex].arriving, tuple->value->cstring);
        schedulesIndex++;
        break;
      }
      case END_SCHEDULES_KEY : {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "reloading schedules menu layer");
        menu_layer_reload_data(schedulesMenuLayer);
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
