#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Airport_Manager.h"

void createAirportArray() {
  numAirports = 0;
  maxAirports = 20;
  airports = (Airport **)malloc(maxAirports*sizeof(Airport));
}

int hasOneStopFlight(Airport * start, Airport * dest) {
  if (start == NULL || dest == NULL || start == dest) {
    return -1;
  }
  int i;
  for (i = 0; i < start->numDestinations; i++) {
    if (start->destinations[i] == dest) {
      return 1;
    }
  }
  return 0;
}

int hasTwoStopFlight(Airport * start, Airport * dest) {
  if (start == NULL || dest == NULL || start == dest) {
    return -1;
  }
  if (hasOneStopFlight(start, dest)) {
    return 2;
  }
  int i;
  for (i = 0; i < start->numDestinations; i++) {
    if (hasOneStopFlight(start->destinations[i], dest)) {
      return 1;
    }
  }
  return 0;
}

int addDestination(Airport * airport, Airport * dest) {
  if (airport == NULL || dest == NULL || airport == dest) {
    return -1;
  }
  int i;
  for(i = 0; i < airport->numDestinations; i++) {
    if (airport->destinations[i] == dest) {
      return 0;
    }
  }
  airport->destinations[airport->numDestinations] = dest;
  airport->numDestinations++;
  return 1;
}

int addAirport (Airport * airport) {
  if (airport == NULL) {
    return -1;
  }
  int i;
  for (i = 0; i < numAirports; i++) {
    if (airports[i] == airport) {
      return 0;
    }
  }
  if (numAirports == maxAirports) {
    maxAirports = maxAirports*2;
    airports = (Airport **)realloc(airports, maxAirports*sizeof(Airport));
  }
  airports[numAirports] = airport;
  numAirports++;
  return 1;
}

Airport * createAirport(const char * name) {
  if (name == NULL) {
    return NULL;
  }
  Airport * a = (Airport *)malloc(sizeof(Airport));
  a->name = (char*)malloc(200*sizeof(char));
  strcpy(a->name, name);
  a->numDestinations = 0;
  a->destinations = (Airport **)malloc(sizeof(Airport));
  return a;
}

void printAirports() {
  printf("NumberOfAirports: %d\n", numAirports);
  int i;
  for(i = 0; i < numAirports; i++) {
    printf("%s\n", airports[i]->name);
  }
  printf("\n");
}

void freeAllAirports() {
  int i;
  for (i = 0; i < numAirports; i++) {
    free(airports[i]->name);
    free(airports[i]->destinations);
    free(airports[i]);
  }
  free(airports);
}


