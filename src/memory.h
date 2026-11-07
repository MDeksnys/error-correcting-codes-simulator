#pragma once

#include <stdlib.h>
#include <string.h>

// Allocates memory safely, exiting the program on failure.
//
// size - number of bytes to allocate
//
// Returns a pointer to the allocated memory.
// Exits with an error message if allocation fails.
void *safe_malloc(size_t size);

// Duplicates a string safely, exiting the program on failure.
//
// str - string to duplicate
//
// Returns a pointer to a newly allocated copy of str.
// Exits with an error message if allocation fails.
char *safe_strdup(char *str);

// Frees a pointer safely and sets it to NULL.
//
// ptr_p - address of the pointer to free
static inline void safe_free_impl(void *ptr_p) {
    void *p;
    memcpy(&p, ptr_p, sizeof p);
    free(p);
    p = NULL;
    memcpy(ptr_p, &p, sizeof p);
}

// Macro wrapper for safe_free_impl, for easier use
#define safe_free(ptr) safe_free_impl(&(ptr))
