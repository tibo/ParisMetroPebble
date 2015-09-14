#include <pebble.h>
#include "models.h"

// Station
Station initStationWithKey(char *key)
{
  Station station;
  station.key = malloc(sizeof(char [strlen(key)+1]));
  station.name = malloc(sizeof(char [0]));
  strcpy(station.key, key);
  return station;
}

void setStationName(Station *station, char *name)
{
  station->name = realloc(station->name, sizeof(char [strlen(name)+1]));
  strcpy(station->name, name);
}

// StationsArray
StationsArray initStationsArray()
{
  StationsArray array;
  array.count = 0;
  array.stations = malloc(sizeof(Station [0]));
  return array;
}

void pushStation(StationsArray *stationsArray, Station *station){
  stationsArray->stations = realloc(stationsArray->stations, sizeof(Station [stationsArray->count+1]));

  stationsArray->stations[stationsArray->count] = *station;

  stationsArray->count++;
}

void destroyStationsArray(StationsArray *stationsArray){
  stationsArray->count = 0;
}

// Destination
Destination initDestinationWithKey(char *key)
{
  Destination destination;
  destination.direction = malloc(sizeof(char [strlen(key)+1]));
  destination.name = malloc(sizeof(char [0]));
  strcpy(destination.direction, key);
  return destination;
}

void setDestinationName(Destination *destination, char *name)
{
  destination->name = realloc(destination->name, sizeof(char [strlen(name)+1]));
  strcpy(destination->name, name);
}

// DestinationsArray
DestinationsArray initDestinationsArray()
{
  DestinationsArray array;
  array.count = 0;
  array.destinations = malloc(sizeof(Destination [0]));
  return array;
}

void pushDestination(DestinationsArray *destinationsArray, Destination *destination)
{
  destinationsArray->destinations = realloc(destinationsArray->destinations, sizeof(Destination [destinationsArray->count+1]));

  destinationsArray->destinations[destinationsArray->count] = *destination;

  destinationsArray->count++;
}

void destroyDestinationsArray(DestinationsArray *destinationsArray)
{
  destinationsArray->count = 0;
}

// Line
Line initLine(char *name)
{
  Line line;
  line.name = malloc(sizeof(char [strlen(name)+1]));
  strcpy(line.name, name);
  line.destinations = initDestinationsArray();
  return line;
}

// LinesArray
LinesArray initLinesArray()
{
  LinesArray array;
  array.count = 0;
  array.lines = malloc(sizeof(Line [0]));
  return array;
}

void pushLine(LinesArray *linesArray, Line *line)
{
  linesArray->lines = realloc(linesArray->lines, sizeof(Line [linesArray->count+1]));

  linesArray->lines[linesArray->count] = *line;

  linesArray->count++;
}

void destroyLinesArray(LinesArray *linesArray)
{
  linesArray->count = 0;
}

// Schedule
Schedule initScheduleWithDestination(char *destinationName)
{
  Schedule schedule;
  schedule.name = malloc(sizeof(char [strlen(destinationName)+1]));
  schedule.arriving = malloc(sizeof(char [0]));
  strcpy(schedule.name, destinationName);
  return schedule; 
}

void setScheduleTime(Schedule *schedule, char *time)
{
  schedule->arriving = realloc(schedule->arriving, sizeof(char [strlen(time)+1]));
  strcpy(schedule->arriving, time);
}

// SchedulesArray
SchedulesArray initSchedulesArray()
{
  SchedulesArray array;
  array.count = 0;
  array.schedules = malloc(sizeof(Schedule [0]));
  return array;
}

void pushSchedule(SchedulesArray *schedulesArray, Schedule *schedule)
{
  schedulesArray->schedules = realloc(schedulesArray->schedules, sizeof(Schedule [schedulesArray->count+1]));

  schedulesArray->schedules[schedulesArray->count] = *schedule;

  schedulesArray->count++;
}

void destroySchedulesArray(SchedulesArray *schedulesArray)
{
  schedulesArray->count = 0;
}