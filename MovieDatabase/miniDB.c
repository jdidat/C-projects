#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "miniDB.h"

char * readData(FILE * f) {
    char * line = (char *) malloc(200 * sizeof(char));
    char * lineCopy = line;
    *lineCopy = fgetc(f);
    while (*lineCopy != EOF && *lineCopy != '\0' && *lineCopy != '\n') {
        lineCopy++;
        *lineCopy = fgetc(f);
    }
    *lineCopy = '\0';
    return line;
}

void createData (FILE * fd) {
  char * line = readData(fd);
  while (line != '\0') {
    char * title = strtok(line, ",");
    if (title == NULL) {
      break;
    }
    char * date = strtok(NULL, ",") + 1;
    char * director = strtok(NULL, ",") + 1;
    int id = atoi(strtok(NULL, ",") + 1);
    createNode(title, director, date, id);
    free(line);
    line = NULL;
    line = readData(fd);
  }
}

void createNode (char * title, char * director, char * date, int id) {
  int i = 0;
  int count = 0;
  char * dateCopy = strdup(date);
  while (date[i] != '\0') {
    if (date[i] == '/') {
      count++;
    }
    i++;
  }
  if (count != 2) {
    return;
  }
  int month = atoi(strtok(dateCopy, "/"));
  int day = atoi(strtok(NULL, "/"));
  if (i != 10 || day > 31 || month > 12 || day == 0 || month == 0) {
    return;
  }
  movie * node = (movie *)malloc(sizeof(movie));
  node->title = strdup(title);
  node->director = strdup(director);
  node->date = strdup(date);
  node->id = id;
  if (head == NULL) {
    head = node;
    return;
  }
  movie * current = head;
  while (current->next != NULL) {
    current = current->next;
  }
  current->next = node;
  node->next = NULL;
}
  
void executeCommands (FILE * fd, FILE * op) {
  char * line = readData(fd);
  char * lineCopy = strdup(line);
  while (line != NULL) {
    char * command = strtok(lineCopy, ",");
    if (command == NULL) {
      break;
    }
    if (strcmp(command, "ADD") == 0) {
      char * title = strtok(NULL, ",") + 1;
      char * date = strtok(NULL, ",") + 1;
      char * director = strtok(NULL, ",") + 1;
      int id = atoi(strtok(NULL, ","));
      addData(title, date, director, id);
    }
    else if (strcmp(command, "EDIT") == 0) {
      int id = atoi(strtok(NULL, ",") + 1);
      char * feature = strtok(NULL, ",") + 1;
      char * data = strtok(NULL, ",") + 1;
      int k = strlen(data);
      data[k - 1] = '\0';
      editData(id, feature, data);
    }
    else if (strcmp(command, "REMOVE") == 0) {
      int id = atoi(strtok(NULL, ",") + 1);
      removeData(id);
    }
    else if (strcmp(command, "LOOKUP") == 0) {
      char * feature = strtok(NULL, ",") + 1;
      char * data = strtok(NULL, ",") + 1;
      int i = strlen(data);
      data[i - 1] = '\0';
      lookupData(feature, data, op);
    }
    else if (strcmp(command, "DISPLAY") == 0) {
      char * feature = strtok(NULL, ",") + 1;
      int order = atoi(strtok(NULL, ",") + 1);
      int max = atoi(strtok(NULL, ",") + 1);
      displayData(feature, order, max, op);
    }
    line = readData(fd);
    lineCopy = strdup(line);
  }
}

void addData (char * title, char * date, char * director, int id) {
  movie * temp = head;
  while (temp != NULL) {
    if(temp->id == id) {
      return;
    }
    temp = temp->next;
  }
  int i = 0;
  int count = 0;
  char * dateCopy = strdup(date);
  while (date[i] != '\0') {
    if (date[i] == '/') {
      count++;
    }
    i++;
  }
  if (count != 2) {
    return;
  }
  int month = atoi(strtok(dateCopy, "/"));
  int day = atoi(strtok(NULL, "/"));
  if (i != 10 || day > 31 || month > 12 || day == 0 || month == 0) {
    return;
  }
  movie * node = (movie *)malloc(sizeof(movie));
  node->title = strdup(title);
  node->date = strdup(date);
  node->director = strdup(director);
  node->id = id;
  node->next = NULL;
  if (head == NULL) {
    head = node;
    return;
  }
  movie * current = head;
  while (current->next != NULL) {
    if (current->id == node->id) {
       return;
    }
    current = current->next;
  }
  current->next = node;
}  

void editData (int id, char * feature, char * data) {
  movie * current = head;
  while (current != NULL) {
    if (current->id == id) {
      if (strcmp(feature, "TITLE") == 0) {
	current->title = strdup(data);
      }
      else if (strcmp(feature, "DIRECTOR") == 0) {
	current->director = strdup(data);
      }
      else if (strcmp(feature, "DATE") == 0) {
	current->date = strdup(data);
      }
    }
    current = current->next;
  }
}

void removeData (int id) {
  movie * previous = NULL;
  movie * current = head;
  while (current != NULL) {
    if(current->id == id) {
      if (previous == NULL) {
	head = head->next;
	free(current);
	current = head;
      }
      else {
	previous->next = current->next;
	free(current);
	current = previous;
      }
    }
    previous = current;
    current = current->next;
  }
}

void lookupData (char * feature, char * data, FILE * op) {
  fprintf(op, "LOOKUP, %s, %s\n", feature, data);
  movie * current = head;
  while (current != NULL) {
    if (strcmp(feature, "TITLE") == 0) {
      if (strcmp(current->title, data) == 0) {
	fprintf(op, "%s, %s, %s, %d\n", current->title, current->date, current->director, current->id);
      }
    }
    if (strcmp(feature, "DIRECTOR") == 0) {
      if (strcmp(current->director, data) == 0) {
	fprintf(op, "%s, %s, %s, %d\n", current->title, current->date, current->director, current->id);
      }
    }
    if (strcmp(feature, "DATE") == 0) {
      if (strcmp(current->date, data) == 0) {
	fprintf(op, "%s, %s, %s, %d\n", current->title, current->date, current->director, current->id);
      }
    }
    current = current->next;
  }
}

movie * findMovie(int id) {
  movie * temp = head;
  while (temp != NULL) {
    if (temp->id == id) {
      return temp;
    }
    temp = temp->next;
  }
  return NULL;
}

void displayData (char * feature, int order, int max, FILE * op) {
  fprintf(op, "DISPLAY, %s, %d, %d\n", feature, order, max);
  int ids[max];
  int i = 0;
  movie * temp = head;
  while (i < max && temp != NULL) {
    ids[i] = temp->id;
    i++;
    temp = temp->next;
  }
  int j;
  int k;
  for (j = 0; j < i; j++) {
    for (k = 0; k < i - 1; k++) {
      if (order == 0 && (strcmp(feature, "TITLE") == 0)) {
        movie * temp = findMovie(ids[k]);
	movie * temp1 = findMovie(ids[k + 1]);
	if (strcmp(temp->title, temp1->title) > 0) {
	  int a = ids[k];
	  ids[k] = ids[k + 1];
	  ids[k + 1] = a;
	}
      }
      if (order == 1 && (strcmp(feature, "TITLE") == 0)) {
	movie * temp = findMovie(ids[k]);
	movie * temp1 = findMovie(ids[k + 1]);
	if (strcmp(temp->title, temp1->title) < 0) {
	  int a = ids[k];
	  ids[k] = ids[k + 1];
	  ids[k + 1] = a;
	}
      }
      if (order == 0 && (strcmp(feature, "DIRECTOR") == 0)) {
	movie * temp = findMovie(ids[k]);
	movie * temp1 = findMovie(ids[k + 1]);
	if (strcmp(temp->director, temp1->director) > 0) {
	  int a = ids[k];
	  ids[k] = ids[k + 1];
	  ids[k + 1] = a;
	}
	if (strcmp(temp->director, temp1->director) == 0 && temp->id > temp1->id) {
	  int a = ids[k];
	  ids[k] = ids[k + 1];
	  ids[k + 1] = a;
	}
      }
      if (order == 1 && (strcmp(feature, "DIRECTOR") == 0)) {
	movie * temp = findMovie(ids[k]);
	movie * temp1 = findMovie(ids[k + 1]);
	if (strcmp(temp->director, temp1->director) < 0) {
	  int a = ids[k];
	  ids[k] = ids[k + 1];
	  ids[k + 1] = a;
	}
	if (strcmp(temp->director, temp1->director) == 0 && temp->id > temp1->id) {
	  int a = ids[k];
	  ids[k] = ids[k + 1];
	  ids[k + 1] = a;
	}
      }
      if (order == 0 && (strcmp(feature, "DATE") == 0)) {
	movie * temp = findMovie(ids[k]);
	movie * temp1 = findMovie(ids[k + 1]);
	char * dateCopy = strdup(temp->date);
	char * dateCopy1 = strdup(temp1->date);
	int tempDay = atoi(strtok(dateCopy, "/"));
	int tempMonth = atoi(strtok(NULL, "/"));
	int tempYear = atoi(strtok(NULL, "/"));
	int temp1Day = atoi(strtok(dateCopy1, "/"));
	int temp1Month = atoi(strtok(NULL, "/"));
	int temp1Year = atoi(strtok(NULL, "/"));
	if (tempYear > temp1Year) {
	  int a = ids[k];
	  ids[k] = ids[k + 1];
	  ids[k + 1] = a;
	}
	else if(tempYear == temp1Year) {
	  if (tempMonth > temp1Month) {
	    int a = ids[k];
	    ids[k] = ids[k + 1];
	    ids[k + 1] = a;
	  }
	  else if (tempMonth == temp1Month) {
	    if (tempDay > temp1Day) {
	      int a = ids[k];
	      ids[k] = ids[k + 1];
	      ids[k + 1] = a;
	    }
	    else if (tempDay == temp1Day) {
	      if (temp->id > temp1->id) {
		int a = ids[k];
		ids[k] = ids[k + 1];
		ids[k + 1] = a;
	      }
	    }
	  }
	}
      }
      if (order == 1 && (strcmp(feature, "DATE") == 0)) {
	movie * temp = findMovie(ids[k]);
	movie * temp1 = findMovie(ids[k + 1]);
	char * dateCopy = strdup(temp->date);
	char * dateCopy1 = strdup(temp1->date);
	int tempDay = atoi(strtok(dateCopy, "/"));
	int tempMonth = atoi(strtok(NULL, "/"));
	int tempYear = atoi(strtok(NULL, "/"));
	int temp1Day = atoi(strtok(dateCopy1, "/"));
	int temp1Month = atoi(strtok(NULL, "/"));
	int temp1Year = atoi(strtok(NULL, "/"));
	if (tempYear < temp1Year) {
	  int a = ids[k];
	  ids[k] = ids[k + 1];
	  ids[k + 1] = a;
	}
	else if (tempYear == temp1Year) {
	  if (tempMonth < temp1Month) {
	    int a = ids[k];
	    ids[k] = ids[k + 1];
	    ids[k + 1] = a;
	  }
	  else if (tempMonth == temp1Month) {
	    if (tempDay < temp1Day) {
	      int a = ids[k];
	      ids[k] = ids[k + 1];
	      ids[k + 1] = a;
	    }
	    else if (tempDay == temp1Day) {
	      if (temp->id < temp1->id) {
		int a = ids[k];
		ids[k] = ids[k + 1];
		ids[k + 1] = a;
	      }
	    }
	  }
	}
      }
    }
  }
  int z;
  movie * tempNode;
  for (z = 0; z < i; z++) {
    tempNode = findMovie(ids[z]);
    fprintf(op, "%s, %s, %s, %d\n", tempNode->title, tempNode->date, tempNode->director, tempNode->id);
  }
}

int main (int argc, char * argv[]) {
  FILE * database = NULL;
  FILE * commands = NULL;
  FILE * output = NULL;
  database = fopen(argv[1], "r+");
  commands = fopen(argv[2], "rb");
  output = fopen(argv[3], "wb");
  createData(database);
  executeCommands(commands, output);
  fclose(database);
  database = fopen(argv[1], "wb");
  movie * current = head;
  while (current != NULL) {
    fprintf(database, "%s, %s, %s, %d\n", current->title, current->date, current->director, current->id);
    current = current->next;
  }
  fclose(database);
  fclose(commands);
  fclose(output);
}
