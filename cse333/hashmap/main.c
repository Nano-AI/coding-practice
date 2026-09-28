#include "stdio.h"
#include "hashmap.h"
#include <stdlib.h>

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
        if (strcmp(res->key, res->value) != 0) {
            printf("ERROR: key(%s) != value(%s)\n", res->key, res->value);
        }
    }

    hashmap_free(map);

    return 0;
}
