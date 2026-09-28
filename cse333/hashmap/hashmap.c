#include "hashmap.h"

unsigned long hash(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++) != '\0') {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash;
}

unordered_map* hashmap_create() {
    node** head = calloc(HASHSIZE_INIT, sizeof(node*));
    // node** head = malloc(sizeof(node*) * HASHSIZE_INIT);
    if (head == NULL) {
        return NULL;
    }
    unordered_map* map = malloc(sizeof(unordered_map));
    if (map == NULL) {
        return NULL;
    }
    map->size = HASHSIZE_INIT;
    map->table = head;
    return map;
}

node* hashmap_lookup(unordered_map *map, char *key) {
    unsigned long index = hash(key) % map->size;
    node* iter = map->table[index];
    while (iter != NULL) {
        if (strcmp(key, iter->key) == 0) {
            return iter;
        }
        iter = iter->next;
    }
    return NULL;
}

node* hashmap_insert(unordered_map *map, char* key, char* value) {
    node* iter = hashmap_lookup(map, key); 
    if (iter == NULL) { // does not exist
        iter = (node*) malloc(sizeof(node));
        if (iter == NULL) {
            return NULL;
        }

        iter->key = strdup(key);
        if (iter->key == NULL) {
            free((void*) iter);
            return NULL;
        }

        iter->value = strdup(value);
        if (iter->value == NULL) {
            free((void*) iter->key);
            free((void*) iter);
            return NULL;
        }

        unsigned long index = hash(key) % map->size;
        iter->next = map->table[index];
        map->table[index] = iter;
        return iter;
    }

    free((void*) iter->value);
    iter->value = strdup(value);
    if (iter->value == NULL) {
        free((void*) iter->key);
        free((void*) iter);
        return NULL;
    }

    return iter;
}

void hashmap_free(unordered_map *map) {
    for (size_t i = 0; i < map->size; ++i) {
        node *it = map->table[i];
        while (it != NULL) {
            node *next = it->next;
            free((void*) it->key);
            free((void*) it->value);
            free((void*) it);
            it = next;
        }
    }
    free((void*) map->table);
    free((void*) map);
}
