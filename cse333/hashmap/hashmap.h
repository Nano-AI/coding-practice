#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE_INIT 32

#define SWAP_PRIMITIVE(a, b, T) { T t = a; a = b; b = t; }

struct node {
    struct node* next;
    const char* key;
    // const char* value;
    int value;
    unsigned long hash;
};

typedef struct node node;

struct unordered_map {
    // pointer to the head of a 1d array of node pointers
    node **table;
    size_t size;
    size_t n;
};

typedef struct unordered_map unordered_map;

/* djb2 */
unsigned long hash(const char *str);

unordered_map* hashmap_create();
unordered_map* hashmap_create_size(size_t size);
node* hashmap_lookup(unordered_map *map, const char *key);
node* hashmap_insert(unordered_map *map, const char *key, int value); // const char *value);
void hashmap_free(unordered_map *map);
void hashmap_resize(unordered_map *map);
void hashmap_print(unordered_map *map);

node** hashmap_sorted(unordered_map *map);

#endif