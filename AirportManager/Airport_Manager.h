#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct Airport{
	char * name; /*The airport name*/
	int numDestinations; /*The number of destinations the airport offers flights to*/
	struct Airport ** destinations; /*The array of destinations the airport offers flights to*/
}Airport;

Airport ** airports; /*The array of all airports managed  by the company*/
int numAirports; /*The current number of airports managed by the company*/
int maxAirports; /*The max number of airports the airports array can store*/

void createAirportArray();
int hasOneStopFlight(Airport * start, Airport * dest);
int hasTwoStopFlight(Airport * start, Airport * dest);
int addDestination(Airport * airport, Airport * dest);
int addAirport(Airport * airport);
Airport * createAirport(const char * name);
void printAirports();
void freeAllAirports();
