#include "hashmap.h"

unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++) != '\0') {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash;
}

unordered_map* hashmap_create() {
    return hashmap_create_size(HASHSIZE_INIT);
}

unordered_map* hashmap_create_size(size_t size) {
    node** head = calloc(size, sizeof(node*));
    if (head == NULL) {
        return NULL;
    }

    unordered_map* map = malloc(sizeof(unordered_map));
    if (map == NULL) {
        free((void*) head);
        return NULL;
    }

    map->n = 0;
    map->size = size;
    map->table = head;
    return map;
}

node* hashmap_lookup(unordered_map *map, const char *key) {
    unsigned long hash_key = hash(key);
    unsigned long index = hash_key % map->size;
    node* iter = map->table[index];
    while (iter != NULL) {
        if (hash_key == iter->hash && strcmp(key, iter->key) == 0) {
            return iter;
        }
        iter = iter->next;
    }
    return NULL;
}

node* hashmap_insert(unordered_map *map, const char* key, int value) {// const char* value) {
    hashmap_resize(map);
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
        iter->hash = hash(iter->key);

        iter->value = value;// strdup(value);

        unsigned long index = hash(key) % map->size;
        iter->next = map->table[index];
        map->table[index] = iter;

        ++map->n;

        return iter;
    }

    iter->value = value;

    return iter;
}

void hashmap_free(unordered_map *map) {
    for (size_t i = 0; i < map->size; ++i) {
        node *it = map->table[i];
        while (it != NULL) {
            node *next = it->next;
            free((void*) it->key);
            free((void*) it);
            it = next;
        }
    }
    free((void*) map->table);
    free((void*) map);
}

void hashmap_resize(unordered_map *map) {
    /*
    n / size >= 0.75 
    n / size >= 3/4
    4*n >= 3*size
    */
    if (4 * map->n < 3 * map->size) return;

    size_t new_size = map->size << 1;
    node **new_table = calloc(new_size, sizeof(node*));

    if (new_table == NULL) {
        return;
    }

    for (size_t i = 0; i < map->size; ++i) {
        node *iter = map->table[i];
        while (iter != NULL) {
            unsigned long new_pos = iter->hash % new_size;
            node *next = iter->next;
            iter->next = new_table[new_pos];
            new_table[new_pos] = iter;
            iter = next;
        }
    }

    free((void*) map->table);
    map->table = new_table;
    map->size = new_size;
}

void hashmap_print(unordered_map *map) {
    printf("{\n");
    node* p = NULL;
    for (size_t i = 0; i < map->size; ++i) {
        node *iter = map->table[i];
        while (iter != NULL) {
            if (p == NULL) p = iter;
            else if (p->value < iter->value) p = iter;
            printf("  \"%s\": %d\n", iter->key, iter->value);
            iter = iter->next;
        }
    }
    printf("}\n");
    printf("max=(\"%s\", %d)\n", p->key, p->value);
    printf("n=%lu, size=%lu\n", map->n, map->size);
    fflush(stdout);
}

static int cmp(const node *a, const node *b) {
    if (a->value != b->value) return b->value - a->value;
    return strcmp(a->key, b->key);
}

size_t partition(node **a, size_t low, size_t high) {
    node *pivot = a[high];
    size_t i = low;                                  
    for (size_t j = low; j < high; ++j)
        if (cmp(a[j], pivot) < 0) { SWAP_PRIMITIVE(a[i], a[j], node*); ++i; }
    SWAP_PRIMITIVE(a[i], a[high], node*);
    return i;
}

void quicksort(node **a, size_t low, size_t high) {  
    if (low >= high) return;
    size_t p = partition(a, low, high);
    if (p > low) quicksort(a, low, p - 1);         
    quicksort(a, p + 1, high);
}

node** hashmap_sorted(unordered_map *map) {
    node **sorted_array = calloc(map->n, sizeof(node*));
    if (sorted_array == NULL) {
        return NULL;
    }

    size_t array_index = 0;

    for (size_t i = 0; i < map->size; ++i) {
        node* iter = map->table[i];
        while (iter != NULL) {
            sorted_array[array_index++] = iter; 
            iter = iter->next;
        }
    }

    quicksort(sorted_array, 0, map->n - 1);

    return sorted_array;
}
