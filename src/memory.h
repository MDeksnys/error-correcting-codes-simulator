#pragma once

#include <stdlib.h>

// Allocates memory safely, exiting the program on failure.
//
// size - number of bytes to allocate
//
// Returns a pointer to the allocated memory.
// Exits with an error message if allocation fails.
void *
safe_malloc(size_t size);

// Duplicates a string safely, exiting the program on failure.
//
// str - string to duplicate
//
// Returns a pointer to a newly allocated copy of str.
// Exits with an error message if allocation fails.
char *
safe_strdup(char *str);

// Frees a pointer safely and sets it to NULL.
//
// ptr_p - address of the pointer to free
void
_safe_free(void **ptr_p);

// Macro wrapper for _safe_free, for easier use
#define safe_free(ptr) _safe_free((void **)&(ptr))
