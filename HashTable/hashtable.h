#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct hashtable_ent {
    char *key; /* Pointer to string key */
    double value; /* Value associated with the key */
    struct hashtable_ent* next; /* Pointer to next node in the chain */
} hashtable_ent_t;

typedef struct hashtable {
    hashtable_ent_t **table; /* Pointer to hashtable array */
    size_t table_len; /* Size of the hashtable array */
} hashtable_t;

extern int hash(const char *key);

hashtable_t *create_hashtable(int max_size);

void free_hashtable(hashtable_t *table);

int get(hashtable_t *table, const char *key, double *value);

int set(hashtable_t *table, const char *key, double value);

int key_exists(hashtable_t *table, const char *key);

int remove_key(hashtable_t *table, const char *key);

#endif
