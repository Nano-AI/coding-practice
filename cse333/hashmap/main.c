#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hashmap.h"

int main() {
    unordered_map *map = hashmap_create();

    char str[10];
    for (int i = 0; i < 200; ++i) {
        sprintf(str, "%d", i);
        hashmap_insert(map, str, str);

        printf("size=%d, n=%d\n", (int) map->size, (int) map->n);
    }

    for (int i = 0; i < 200; ++i) {
        sprintf(str, "%d", i);
        node* res = hashmap_lookup(map, str);
        assert(res != NULL && strcmp(res->key, res->value) == 0);
    }

    hashmap_free(map);

    return 0;
}
