#include <pebble.h>

typedef struct {
  char name[32]; // Station name
  char key[32]; // Station key
} __attribute__((packed)) Station;