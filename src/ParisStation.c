#include <pebble.h>
#include "StationsWindow.c.inc"

// init/deinit
static void init(void) {
  Window *stationsWindow = init_station_window();
  window_stack_push(stationsWindow, true);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", stationsWindow);
}

static void deinit(void) {
  // window_destroy(stationsWindow);
  // app_sync_deinit(&s_sync);
}

// main
int main(void) {
  init();
  app_event_loop();
  deinit();
}
