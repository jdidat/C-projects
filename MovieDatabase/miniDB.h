#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct movie {
  char * title;
  char * date;
  char * director;
  int id;
  struct movie * next;
} movie;

movie * head;

char * readData(FILE * fd);
void createData(FILE * fd);
void createNode(char * title, char * director, char * date, int id);
void executeCommands(FILE * fd, FILE * op);
void addData(char * title, char * date, char * director, int id);
void editData(int id, char * feature, char * data);
void removeData(int id);
movie * findMovie(int id);
void lookupData(char * feature, char * data, FILE * op);
void displayData(char * feature, int order, int max, FILE * op);
int main(int argc, char * argv[]);
