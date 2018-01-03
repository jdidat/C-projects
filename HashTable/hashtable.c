#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

static hashtable_ent_t* add_new(hashtable_ent_t** entry, const char* key) {
    if (entry == NULL || key == NULL) return NULL;
    *entry = (hashtable_ent_t*)malloc(sizeof(hashtable_ent_t));
    if (*entry == NULL) return NULL;
    (*entry)->key = strdup(key);
    //strcpy((*entry)->key, key);
    if ((*entry)->key == NULL) {
        free(*entry);
        return NULL;
    }
    (*entry)->next = NULL;
    return *entry;
}

/*
 * Helper function that does most of the implemention work
 */
static hashtable_ent_t* locate(hashtable_t* ht, const char* key, int create_if_missing) {
    if (ht == NULL || key == NULL) return NULL;
    int h = hash(key) % ht->table_len;
    hashtable_ent_t* node = ht->table[h];
    if (node != NULL) {
        /* Search until we find a match or hit the end */
        while (node->next != NULL && strcmp(key, node->key)) node = node->next;
        if (!strcmp(key, node->key)) {
            return node;
        } else if (node->next == NULL && create_if_missing) {
            /* If we failed to find the key, we can create an entry in place */
            return add_new(&node->next, key);
        }
    } else if (create_if_missing) {
        return add_new(&ht->table[h], key);
    }
    return NULL;
}

hashtable_t *create_hashtable(int max_size) {
  hashtable_t * ht = (hashtable_t *)malloc(sizeof(hashtable_t));
  ht->table_len = max_size;
  ht->table = (hashtable_ent_t **)malloc(sizeof(hashtable_ent_t)*max_size);
  int i;
  for(i = 0; i < ht->table_len; i++) {
    ht->table[i] = NULL;
  }
  return ht;
}

void free_hashtable(hashtable_t *ht) {
  int i;
  for(i = 0; i < ht->table_len; i++) {
    hashtable_ent_t * temp = ht->table[i];
    hashtable_ent_t * temp1 = ht->table[i];
    while (temp != NULL) {
      temp1 = temp;
      temp = temp->next;
      //free(temp1->key);
      free(temp1);
      temp1 = NULL;
    }
  }
  free(ht->table);
  free(ht);
}

int get(hashtable_t *ht, const char *key, double *value) {
  if (ht == NULL || key == NULL) {
    return -1;
  }
  hashtable_ent_t * temp = locate(ht, key, 0);
  if (temp == NULL) {
    return -1;
  }
  while(temp->next != NULL && strcmp(temp->key, key)) {
    temp = temp->next;
  }
  if (temp == NULL) {
    return -1;
  }
  *value = temp->value;
  return 0;
}

int set(hashtable_t *ht, const char *key, double value) {
  if (ht == NULL || key == NULL) {
    return -1;
  }
  hashtable_ent_t * temp = locate(ht, key, 0);
  if (temp == NULL) {
    temp = locate(ht, key, 1);
    temp->value = value;
    temp->next = NULL;
    return 0; 
  }
  else {
    while (temp->next != NULL) {
      temp = temp->next;
    }
  }
  temp->next = (hashtable_ent_t *)malloc(sizeof(hashtable_ent_t));
  temp->next->value = value;
  temp->next->key = (char *)malloc(sizeof(char)*(strlen(key)+1));
  strcpy(temp->next->key, key);
  temp->next->next = NULL;
  return 0;
}

int key_exists(hashtable_t *ht, const char *key) {
  if(ht == NULL || key == NULL) {
    return -1;
  }
  hashtable_ent_t * temp = locate(ht, key, 0);
  while (temp != NULL && strcmp(temp->key, key)) {
    temp = temp->next;
  }
  if (temp == NULL) {
    return 0;
  }
  return 1;
}

int remove_key(hashtable_t *ht, const char *key) {
  if(ht == NULL || key == NULL) {
    return -1;
  }
  hashtable_ent_t * head = locate(ht, key, 0);
  if (head == NULL) {
    return -1;
  }
  hashtable_ent_t * temp;
  if (head != NULL && (strcmp(head->key, key)) == 0) {
    temp = head;
    head = head->next;
    free(temp->key);
    free(temp);
    return 0;
  }
  while (head != NULL && (strcmp(head->key, key)) == 0) {
    temp = head;
    head->next = temp->next;
    //temp = temp->next;
  }
  if (temp == NULL) {
    return -1;
  }
  temp->next = head->next;
  free(head->key);
  free(head);
  return 0;
}
