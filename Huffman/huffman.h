#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list {
  char * key;
  char * value;
  struct list * next;
} list;

typedef struct tree {
  char * key;
  int bit;
  struct tree * left;
  struct tree * right;
} tree;

tree * top;
list * head;
int place;

void retrieveLine (const char * code);
void splitLine (char * word);
void createList (char * key, char * value);
tree * createTree (tree * top, char * key, char * value);
void printTreeNode(tree * root);
list * locateNode (char c);
void compress (const char * inputFile, const char * outputFile);
void decompress (const char * inputFile, const char * outputFile);
void deleteTree (tree * top);
void deleteList ();

