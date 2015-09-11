#include <pebble.h>

typedef struct {
  char *name;
  char *key;
} __attribute__((packed)) Station;

Station initStationWithKey(char *key);
void setStationName(Station *station, char *name);

typedef struct {
  char *name;
  char *direction;
} __attribute__((packed)) Destination;

typedef struct {
  char *name;
  Destination destinations[2];
} __attribute__((packed)) Line;

typedef struct {
  char *name;
  char *arriving;
} __attribute__((packed)) Schedule;


// Arrays
typedef struct {
  Station *stations;
  int count;
} __attribute__((packed)) StationsArray;

StationsArray initStationsArray();
void pushStation(StationsArray *stationsArray, Station *station);
void destroyStationsArray(StationsArray *stationsArray);