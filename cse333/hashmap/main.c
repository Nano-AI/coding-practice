#include "stdio.h"
#include "hashmap.h"

int main() {
    unordered_map *map = hashmap_create();
    hashmap_insert(map, "Fortnite", "Balls");
    hashmap_insert(map, "Test", " number one");
    hashmap_insert(map, "Test", "icles");

    node* res = hashmap_lookup(map, "Fortnite");
    printf("%s : %s\n", res->key, res->value);
    res = hashmap_lookup(map, "Test");
    printf("%s%s\n", res->key, res->value);

    hashmap_free(map);

    return 0;
}
