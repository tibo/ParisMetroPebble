#include <pebble.h>

typedef struct {
  char name[32];
  char direction[1];
} __attribute__((packed)) Destination;

typedef struct {
  char name[2];
  Destination destinations[2];
} __attribute__((packed)) Line;