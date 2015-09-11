#include <pebble.h>
#include "models.h"

// Station
Station initStationWithKey(char *key)
{
  Station station;
  station.key = malloc(sizeof(char [strlen(key)+1]));
  strcpy(station.key, key);
  return station;
}

void setStationName(Station *station, char *name)
{
  station->name = malloc(sizeof(char [strlen(name)+1]));
  strcpy(station->name, name);
}

// StationsArray
StationsArray initStationsArray(){
  StationsArray array;
  array.count = 0;
  array.stations = malloc(sizeof(Station [0]));
  return array;
}

void pushStation(StationsArray *stationsArray, Station *station){
  stationsArray->stations = realloc(stationsArray->stations, sizeof(Station[stationsArray->count+1]));

  stationsArray->stations[stationsArray->count] = *station;

  stationsArray->count++;
}

void destroyStationsArray(StationsArray *stationsArray){
  free(stationsArray);
}