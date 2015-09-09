#include <pebble.h>

typedef struct {
  char *name;
  char *direction;
} __attribute__((packed)) Destination;

typedef struct {
  char *name;
  Destination destinations[2];
} __attribute__((packed)) Line;