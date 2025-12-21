#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *
safe_malloc(size_t size) {
  void *p = malloc(size);

  if (p == NULL) {
    fprintf(stderr, "Out of memory!\n");
    exit(EXIT_FAILURE);
  }

  return p;
}

char *
safe_strdup(char *str) {
  char *p = strdup(str);

  if (p == NULL) {
    fprintf(stderr, "Out of memory!\n");
    exit(EXIT_FAILURE);
  }

  return p;
}

void
_safe_free(void **ptr_p) {
  if (ptr_p == NULL)
    return;

  free(*ptr_p);
  *ptr_p = NULL;
}
