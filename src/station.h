#include <pebble.h>

typedef struct {
  char name[32];
  char key[32];
} __attribute__((packed)) Station;