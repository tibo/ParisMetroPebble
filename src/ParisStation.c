#include <pebble.h>
#include "models.h"
#include "StationsWindow.c.inc"
#include "appmessage.h"

// app message
void message_received(DictionaryIterator *iter, void *context) {
  Tuple *tuple = dict_read_first(iter);
  while (tuple) {
    switch(tuple->key){
      case NEW_STATION_KEY: {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "new station key: %s", tuple->value->cstring);
        Station station = initStationWithKey(tuple->value->cstring);
        pushStation(&stations, &station);
        break;
      }
      case NEW_STATION_NAME: {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "new station name: %s", tuple->value->cstring);
        setStationName(&stations.stations[stations.count-1], tuple->value->cstring);
        break;
      }
      case END_STATIONS_KEY: {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "reloading stations menu layer");
        menu_layer_reload_data(stationsMenuLayer);
        break;
      }
      case NEW_LINE_KEY : {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "new line: %s length: %i %i", tuple->value->cstring, strlen(tuple->value->cstring), sizeof(char [strlen(tuple->value->cstring)]));
        Line line = initLine(tuple->value->cstring);
        pushLine(&lines, &line);
        break;
      }
      case NEW_DESTINATION_DIRECTION_KEY : {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "new destination: %s", tuple->value->cstring);
        Destination destination = initDestinationWithKey(tuple->value->cstring);
        pushDestination(&lines.lines[lines.count-1].destinations, &destination);
        break;
      }
      case NEW_DESTINATION_NAME_KEY : {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "new destination name: %s length: %i %i", tuple->value->cstring, strlen(tuple->value->cstring), sizeof(char [strlen(tuple->value->cstring)]));
        int destinationIndex = lines.lines[lines.count-1].destinations.count-1;
        setDestinationName(&lines.lines[lines.count-1].destinations.destinations[destinationIndex], tuple->value->cstring);
        break;
      }
      case END_DESTINATIONS_KEY : {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "reloading destinations menu layer");
        menu_layer_reload_data(destinationsMenuLayer);
        break;
      }
      case NEW_SCHEDULE_DESTINATION_KEY : {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "new schedule destination: %s", tuple->value->cstring);
        Schedule schedule = initScheduleWithDestination(tuple->value->cstring);
        pushSchedule(&schedules, &schedule);
        break;
      }
      case NEW_SCHEDULE_TIME_KEY : {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "new schedule time: %s", tuple->value->cstring);
        setScheduleTime(&schedules.schedules[schedules.count-1], tuple->value->cstring);
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
