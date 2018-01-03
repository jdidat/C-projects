

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "best_team.h"

// Position meanings
const char * position_name[5] = {"Point Guard", 
    "Shooting Guard", 
    "Small Forward",
    "Power Forward",
    "Center"};

Team * initializeTeam() {
  Team * t;
  t = (Team*)malloc(sizeof(Team));
  t->_numOfPlayers = 0;
  t->_players[0] = NULL;
  return t;
}
char * inputFile(FILE *fd) {
    char * buffer = (char *) malloc(200 * sizeof(char));
    char * bufferptr = buffer;
    *bufferptr = fgetc(fd);
    while (*bufferptr != EOF && *bufferptr != '\0' && *bufferptr != '\n') {
        bufferptr++;
        *bufferptr = fgetc(fd);
    }
    *bufferptr = '\0';
    return buffer;
}

Player * parsePlayer(char * str) {
  Player * p;
  p = (Player*)malloc(sizeof(Player));
  p->_name = (char*)malloc(200*sizeof(char));
  strcpy(p->_name, str + 18);
  int shirtNum = atoi(strtok(str, ","));
  p->_shirtNum = shirtNum;
  int age = atoi(strtok(NULL, ","));
  p->_age = age;
  char * pos = strtok(NULL, ",");
  if (atoi(pos) == 0) {
    p->_pos = PG;
  }
  if (atoi(pos) == 1) {
    p->_pos = SG;
  }
  if (atoi(pos) == 2) {
    p->_pos = SF;
  }
  if (atoi(pos) == 3) {
    p->_pos = PF;
  }
  if (atoi(pos) == 4) {
    p->_pos = C;
  }
  int pass = atoi(strtok(NULL, ","));
  int shoot = atoi(strtok(NULL, ","));
  int speed = atoi(strtok(NULL, ","));
  int block = atoi(strtok(NULL, ","));
  int height = atoi(strtok(NULL, ","));
  stats s;
  s._pass = pass;
  s._shoot = shoot;
  s._speed = speed;
  s._block = block;
  s._height = height;
  p->_stats = s;
  return p;
}
void loadData(FILE * fd) {
  // TODO: load all data from file to allPlayers
  // Don't forget to free the buffer returned by inputFile
  char * f = inputFile(fd);
  Player * p = parsePlayer(f);
  Player * c = p;
  Player * first = c;
  free(f);
  f = inputFile(fd);
  while (*f != '\0') {
    p = parsePlayer(f);
    c->_next = p;
    c = c->_next;
    free(f);
    f = inputFile(fd);
  }
  free(f);
  c->_next = NULL;
  allPlayers = first;
}
Player * findBestPlayer(int pos) {
    // TODO: iterate through allPlayers
  Player * temp = allPlayers;
  Player * best = temp;
  while (temp->_next != NULL) {
      if (pos == 0) {
	if (temp->_next->_stats._pass > best->_stats._pass) {
	  best = temp->_next;
	}
      }
      if (pos == 1) {
	if (temp->_next->_stats._shoot > best->_stats._shoot) {
	  best = temp->_next;
	}
      }
      if (pos == 2) {
	if (temp->_next->_stats._speed > best->_stats._speed) {
	  best = temp->_next;
	}
      }
      if (pos == 3) {
	if (temp->_next->_stats._block > best->_stats._block) {
	  best = temp->_next;
	}
      }
      if (pos == 4) {
	if (temp->_next->_stats._height > best->_stats._height) {
	  best = temp->_next;
	}
      }
    temp = temp->_next;
  }
  return best;
}

int buildBestTeam(Team * t) {
  // TODO: utilize findBestPlayer(pos) to build the best team 
  // order of players has to be PG SG SF PF C
  int i = 0;
  for (i = 0; i < 5; i++) {
    t->_players[i] = findBestPlayer(i);
    t->_numOfPlayers++;
  }
  return t->_numOfPlayers;
}

void freePlayer(Player * p) {
  // TODO: free allocated memory in p
  free(p->_name);
  free(p);
}

void freeTeam(Team * t) {
  // TODO: free allocated memory in t, utilize freePlayer(p)
  int i;
  for (i = 0; i < t->_numOfPlayers; i++) {
    freePlayer(t->_players[i]);
    t->_players[i] = NULL;
  }
  free(t);
  t = NULL;
  allPlayers = NULL;
}
void print(Team * t) {
    printf("=========================\n");
    printf("      The Best Team      \n");
    printf("=========================\n\n");
    int i;
    for (i = 0; i < t->_numOfPlayers; i++) {
        printf("Name     : %s\n", t->_players[i]->_name);
        printf("Age      : %d\n", t->_players[i]->_age);
        printf("Shirt #  : %d\n", t->_players[i]->_shirtNum);
        printf("Position : %s\n\n", position_name[t->_players[i]->_pos]);
    }
}
