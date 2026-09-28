#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../hashmap/hashmap.h"

bool is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int main() {
    unordered_map *words = hashmap_create();
    
    FILE *fptr = fopen("pride.txt", "r");
    if (fptr == NULL) return -1;

    char *line = NULL;

    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fptr)) != -1) {
        size_t left = 0, right = 0;
        size_t n = (size_t) read;
        while (left < n) {
            while (left < n && !is_letter(line[left])) { ++left; }
            right = left;

            while (right < n && is_letter(line[right])) {
                line[right] = tolower(line[right]);
                ++right;
            }

            if (left < right) {
                char temp = line[right];
                line[right] = '\0';

                node *res = hashmap_lookup(words, &line[left]);
                if (res == NULL) {
                    hashmap_insert(words, &line[left], 1);
                } else {
                    ++res->value;
                }

                line[right] = temp;
            }

            left = ++right;
        }
    }

    node** sorted = hashmap_sorted(words);
    for (size_t i = 0; i < 10; ++i) {
        printf("%s: %d\n", sorted[i]->key, sorted[i]->value);
    }

    // hashmap_print(words);
    hashmap_free(words);

    free((void*) sorted);
    free((void*) line);
    fclose(fptr);
    return 0;
}
