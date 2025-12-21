#include "bit_string.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "memory.h"

uint8_t
vectors_from_bit_string(char ***vectors_p,
                        char *input,
                        size_t input_length,
                        size_t vector_count,
                        uint8_t vector_length) {
  char **vectors = *vectors_p;
  uint8_t append_count = 0;

  for (size_t i = 0; i < vector_count; i++) {}

  for (size_t i = 0; i < vector_count; i++) {
    char *vector = safe_malloc(vector_length + 1);

    for (uint8_t j = 0; j < vector_length; j++) {
      if (i * vector_length + j >= input_length) {
        vector[j] = '0';
        append_count++;

        continue;
      }

      vector[j] = input[i * vector_length + j];
    }

    vector[vector_length] = '\0';
    vectors[i] = vector;
  }

  return append_count;
}

char *
bit_string_from_vectors(char **vectors,
                        uint8_t count_appended,
                        size_t vector_count,
                        uint8_t vector_length) {
  size_t length = vector_count * vector_length - count_appended;
  char *result = safe_malloc(length + 1);

  for (size_t i = 0; i < vector_count; i++) {
    for (uint8_t j = 0; j < vector_length; j++) {
      size_t index = i * vector_length + j;

      if (index >= length)
        goto end_loop;

      result[index] = vectors[i][j];
    }
  }
end_loop:

  result[length] = '\0';
  return result;
}

char *
bytes_from_bit_string(char *bit_str) {
  assert(strlen(bit_str) % 8 == 0);

  size_t length = strlen(bit_str) / 8;
  char *buffer = safe_malloc(length + 1);

  for (size_t i = 0; i < length; i++) {
    buffer[i] = 0;

    for (uint8_t j = 0; j < 8; j++) {
      uint8_t bit = 1 << j;
      char c = bit_str[i * 8 + j];
      buffer[i] |= (c == '0' ? 0 : bit);
    }
  }

  buffer[length] = '\0';
  return buffer;
}

char *
bit_string_from_bytes(char *str, size_t length) {
  char *buffer = safe_malloc(length * 8 + 1);

  for (size_t i = 0; i < length; i++) {
    char byte = str[i];

    for (uint8_t j = 0; j < 8; j++) {
      uint8_t bit = (byte >> j) & 1;
      buffer[i * 8 + j] = '0' + bit;
    }
  }

  buffer[length * 8] = '\0';
  return buffer;
}
