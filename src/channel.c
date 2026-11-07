#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

uint64_t
channel_error_vector(double error_probability, uint8_t length) {
  uint64_t error = 0;

  for (uint8_t i = 0; i < length; i++)
    if (((double)rand() / (double)RAND_MAX) < error_probability)
      error |= (uint64_t)1 << i;

  return error;
}

uint64_t *
channel_send_vectors(uint64_t *vectors,
                     size_t vector_count,
                     double error_probability,
                     uint8_t vector_length) {
  uint64_t *result = safe_malloc(vector_count * sizeof(uint64_t));

  for (size_t i = 0; i < vector_count; i++) {
    uint64_t error_vector = channel_error_vector(error_probability, vector_length);
    result[i] = vectors[i] ^ error_vector;
  }

  return result;
}
