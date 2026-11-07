#include "memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *safe_malloc(size_t size) {
    void *p = malloc(size);

    if (p == NULL) {
        fprintf(stderr, "Out of memory!\n");
        exit(EXIT_FAILURE);
    }

    return p;
}

char *safe_strdup(char *str) {
    char *p = strdup(str);

    if (p == NULL) {
        fprintf(stderr, "Out of memory!\n");
        exit(EXIT_FAILURE);
    }

    return p;
}
