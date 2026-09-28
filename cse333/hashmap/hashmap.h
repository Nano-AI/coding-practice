#ifndef HASHMAP_H
#define HASHMAP_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"


#define HASHSIZE_INIT 101

struct node {
    struct node* next;
    const char* key;
    const char* value;
};

typedef struct node node;

struct unordered_map {
    // pointer to the head of a 1d array of node pointers
    node **table;
    size_t size;
};

typedef struct unordered_map unordered_map;

/* djb2 */
unsigned long hash(char *str);

unordered_map* hashmap_create();
node* hashmap_lookup(unordered_map *map, char *key);
node* hashmap_insert(unordered_map *map, char *key, char *value);
void hashmap_free(unordered_map *map);

#endif