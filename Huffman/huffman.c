#include "huffman.h"

void retrieveLine (const char * code) {
  FILE * f = fopen(code, "rb");
  char word[1000];
  while (fgets(word, sizeof(char)*200, f)) {
    splitLine(word);
  }
  fclose(f);
}

void splitLine (char * word) {
  char * key = strtok(word, "=");
  char * value = strtok(NULL, "=");
  char * term = strchr(value, '\n');
  if (term != NULL) {
    *term = '\0';
  }
  createList(key, value);
  createTree(top, key, value);
}

void createList (char * key, char * value) {
  list * node = (list *)malloc(sizeof(list));
  node->key = (char *)malloc(200*sizeof(char));
  strcpy(node->key, key);
  node->value = (char *)malloc(200*sizeof(char));
  strcpy(node->value, value);
  if (head == NULL) {
    head = node;
    return;
  }
  list * previous = head;
   list * current = head->next;
  while (current != NULL) {
    previous = current;
    current = current->next;
  }
  current = node;
  previous->next = current;
}

tree * createTree (tree * top, char * key, char * value) {
  tree * node = (tree *)malloc(sizeof(tree));
  if (value[place] == '\0') {
    place = 0;
    return NULL;
  }
  node->bit = value[place] - '0';
  node->key = NULL;
  node->left = NULL;
  node->right = NULL;
  if (node->bit == 0 && top->left == NULL) {
    if (value[place + 1] == '\0') {
      node->key = (char *)malloc(200*sizeof(char));
      strcpy(node->key, key);
    }
    top->left = node;
    place++;
    createTree(top->left, key, value);
  }
  else if (node->bit == 1 && top->right == NULL) {
    if (value[place + 1] == '\0') {
      node->key = (char *)malloc(200*sizeof(char));
      strcpy(node->key, key);
    }
    top->right = node;
    place++;
    createTree(top->right, key, value);
  }
  else {
    if (node->bit == 0) {
      place++;
      createTree(top->left, key, value);
    }
    else {
      place++;
      createTree(top->right, key, value);
    }
  }
  return NULL;
}

list * locateNode (char c) {
  list * current = head;
  while (current != NULL) {
    if (current->key[0] == c) {
      return current;
    }
    current = current->next; 
  }
  return NULL;
}

void compress (const char * inputFile, const char * outputFile) {
  FILE * in = fopen(inputFile, "rb");
  FILE * out = fopen(outputFile, "wb");
  char * w = (char *)malloc(999999*sizeof(char));
  int val;
  unsigned char c;
  int length;
  int count = 0;
  int count1 = 0;
  while ((val = fgetc(in)) != EOF) {
    c = val;
    list * temp = locateNode(c);
    if (temp == NULL) {
      break;
    }
    int len = strlen(temp->value);
    for(length = 0; length < len; length++) {
      w[count] = temp->value[length];
      count++;
    }
  }
  w[count] = '\0';
  int i;
  int j;
  for (i = 0; i < count; i += 8) {
    unsigned char bit = 0;
    for (j = i; j < i + 8; j++) {
      if (w[j] == '\0') {
        break;
      }
      bit = bit | (w[j] - '0');
      if (count1 != 7) {
	bit = bit << 1;
      }
      count1++;
    }
    if (count1 != 7) {
      bit = bit << (7 - count1);
    }
    count1 = 0;
    fputc(bit, out);
  }
  fclose(in);
  fclose(out);
}

void decompress (const char * inputFile, const char * outputFile) {
  FILE * in = fopen(inputFile, "rb");
  FILE * out = fopen(outputFile, "wb");
  tree * current = top;
  char * w = (char *)malloc(999999*sizeof(char));
  int count = 0;
  int i;
  int j;
  int k;
  char c;
  int val;
  while (1) {
    val = fgetc(in);
    c = val;
    if (c == EOF) {
      if (((c = fgetc(in)) == EOF) && ((c = fgetc(in)) == EOF) && ((c = fgetc(in)) == EOF)) {
	break;
      }
      rewind(in);
      for (k = 0; k < count/8 + 1; k++) {
	c = fgetc(in);
      }
    }
    for (i = 7; i >= 0; i--) {
      w[count] = ((c >> i) & 1) + '0';
      count++;
    }
  }
  w[count] = '\0';
  for (j = 0; w[j] != '\0'; j++) {
      if (w[j] - '0' == 0) {
	current = current->left;
	if (current == NULL) {
	  return;
	}
	if (current->key != NULL) {
	  fputc(current->key[0], out);
	  current = top;
	}
      }
      else {
	current = current->right;
	if (current == NULL) {
	  return;
	}
	if (current->key != NULL) {
	  fputc(current->key[0], out);
	  current = top;
	}
      }
  }
  fclose(in);
  fclose(out);
}


void deleteTree(tree * node) {
  if (node == NULL) {
    return;
  }
  deleteTree(node->right);
  deleteTree(node->left);
  free(node->key);
  free(node);
}

void deleteList() {
  list * temp = head;
  list * temp1;
  while(temp != NULL) {
    temp1 = temp;
    temp = temp->next;
    free(temp1->key);
    free(temp1->value);
    free(temp1);
  }
}

int main (int argc, const char * argv[]) {
  top = (tree *)malloc(sizeof(tree));
  top->bit = 2;
  top->right = NULL;
  top->left = NULL;
  if (argc == 5) {
    if (strcmp(argv[1], "-c") == 0) {
      retrieveLine(argv[4]);
      compress(argv[2], "out.huf");
    }
    else if (strcmp(argv[3], "-c") == 0) {
      retrieveLine(argv[2]);
      compress(argv[4], "out.huf");
    }
    else if (strcmp(argv[1], "-d") == 0) {
      retrieveLine(argv[4]);
      decompress(argv[2], "out.dec");
    }
    else if (strcmp(argv[3], "-d") == 0) {
      retrieveLine(argv[2]);
      decompress(argv[4], "out.dec");
    }
  }
  else if (argc == 7) {
    if (strcmp(argv[1], "-c") == 0) {
      if (strcmp(argv[3], "-p") == 0) {
	retrieveLine(argv[6]);
	compress(argv[2], argv[4]);
      }
      else if (strcmp(argv[5], "-p") == 0) {
	retrieveLine(argv[4]);
	compress(argv[2], argv[6]);
      }
    }
    else if (strcmp(argv[3], "-c") == 0) {
      if (strcmp(argv[1], "-p") == 0) {
	retrieveLine(argv[6]);
	compress(argv[4], argv[2]);
      }
      else if (strcmp(argv[5], "-p") == 0) {
	retrieveLine(argv[4]);
	compress(argv[4], argv[6]);
      }
    }
    else if (strcmp(argv[5], "-c") == 0) {
      if (strcmp(argv[1], "-p") == 0) {
	retrieveLine(argv[4]);
	compress(argv[6], argv[2]);
      }
      else if (strcmp(argv[3], "-p") == 0) {
	retrieveLine(argv[4]);
	compress(argv[6], argv[4]);
      }
    }
    if (strcmp(argv[1], "-d") == 0) {
      if (strcmp(argv[3], "-p") == 0) {
	retrieveLine(argv[6]);
	decompress(argv[2], argv[4]);
      }
      else if (strcmp(argv[5], "-p") == 0) {
	retrieveLine(argv[4]);
	decompress(argv[2], argv[6]);
      }
    }
    else if (strcmp(argv[3], "-d") == 0) {
      if (strcmp(argv[1], "-p") == 0) {
	retrieveLine(argv[6]);
	decompress(argv[4], argv[2]);
      }
      else if (strcmp(argv[5], "-p") == 0) {
	retrieveLine(argv[4]);
	decompress(argv[4], argv[6]);
      }
    }
    else if (strcmp(argv[5], "-d") == 0) {
      if (strcmp(argv[1], "-p") == 0) {
	retrieveLine(argv[4]);
	decompress(argv[6], argv[2]);
      }
      else if (strcmp(argv[3], "-p") == 0) {
	retrieveLine(argv[4]);
	decompress(argv[6], argv[4]);
      }
    }
  }
  deleteList();
  deleteTree(top);
  return 0;
}
