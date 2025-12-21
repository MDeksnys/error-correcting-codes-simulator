#include "encode.h"

#include <assert.h>
#include <string.h>

#include "matrix.h"
#include "memory.h"

char *
encode(Matrix *matrix_p, char *vec) {
  assert(vec != NULL);
  assert(matrix_is_valid(matrix_p));
  assert(strlen(vec) == matrix_p->row_count);

  return vector_matrix_mul(vec, matrix_p);
}

char **
encode_all(Matrix *matrix_p, char **vectors, size_t vec_count) {
  char **result = safe_malloc(vec_count * sizeof(char *));

  for (size_t i = 0; i < vec_count; i++)
    result[i] = encode(matrix_p, vectors[i]);

  return result;
}
