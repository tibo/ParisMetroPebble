
static Window *schedulesWindow;
static char *direction;

// window
static void schedules_window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "schedules window for key: %s", direction);
}

static void schedules_window_unload(Window *window) {
  window_destroy(window);
}

// init
Window *init_schedules_window(char *directionKey) {
  direction = directionKey;
  schedulesWindow = window_create();
  window_set_window_handlers(schedulesWindow, (WindowHandlers) {
    .load = schedules_window_load,
    .unload = schedules_window_unload,
  });
  return schedulesWindow;
}