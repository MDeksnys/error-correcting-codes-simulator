#include <stdint.h>
#include <stdio.h>

#include "memory.h"
#include "vector.h"

char *
channel_error_vector(double error_probability, uint8_t length) {
  char *error_vector = safe_malloc(length + 1);

  for (uint8_t i = 0; i < length; i++)
    error_vector[i] = (double)rand() / (double)RAND_MAX < error_probability ? '1' : '0';
  error_vector[length] = '\0';

  return error_vector;
}

char **
channel_send_vectors(char **vectors,
                     size_t vector_count,
                     double error_probability,
                     uint8_t vector_length) {
  char **result = safe_malloc(vector_count * sizeof(char *));

  for (size_t i = 0; i < vector_count; i++) {
    char *error_vector = channel_error_vector(error_probability, vector_length);
    result[i] = vector_add(vectors[i], error_vector, vector_length);
    safe_free(error_vector);
  }

  return result;
}
