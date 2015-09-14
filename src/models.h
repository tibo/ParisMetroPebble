#include <pebble.h>

// Stations
typedef struct {
  char *name;
  char *key;
} __attribute__((packed)) Station;

Station initStationWithKey(char *key);
void setStationName(Station *station, char *name);

typedef struct {
  Station *stations;
  int count;
} __attribute__((packed)) StationsArray;

StationsArray initStationsArray();
void pushStation(StationsArray *stationsArray, Station *station);
void destroyStationsArray(StationsArray *stationsArray);

// Destinations
typedef struct {
  char *name;
  char *direction;
} __attribute__((packed)) Destination;

Destination initDestinationWithKey(char *key);
void setDestinationName(Destination *destination, char *name);

typedef struct {
  Destination *destinations;
  int count;
} __attribute__((packed)) DestinationsArray;

DestinationsArray initDestinationsArray();
void pushDestination(DestinationsArray *destinationsArray, Destination *destination);
void destroyDestinationsArray(DestinationsArray *destinationsArray);

typedef struct {
  char *name;
  DestinationsArray destinations;
} __attribute__((packed)) Line;

Line initLine(char *name);

typedef struct {
  Line *lines;
  int count;
} __attribute__((packed)) LinesArray;

LinesArray initLinesArray();
void pushLine(LinesArray *linesArray, Line *line);
void destroyLinesArray(LinesArray *linesArray);

// Schedules
typedef struct {
  char *name;
  char *arriving;
} __attribute__((packed)) Schedule;

Schedule initScheduleWithDestination(char *destinationName);
void setScheduleTime(Schedule *schedule, char *time);

typedef struct {
  Schedule *schedules;
  int count;
} __attribute__((packed)) SchedulesArray;

SchedulesArray initSchedulesArray();
void pushSchedule(SchedulesArray *schedulesArray, Schedule *schedule);
void destroySchedulesArray(SchedulesArray *schedulesArray);