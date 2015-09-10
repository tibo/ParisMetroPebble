#include <pebble.h>

typedef struct {
  char *name;
  char *arriving;
} __attribute__((packed)) Schedule;