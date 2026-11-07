#include "bit_string.h"

#include <stdint.h>
#include <string.h>

#include "memory.h"

void
vectors_from_string(uint64_t *bit_vecs,
                    const char *input,
                    size_t input_length,
                    size_t vector_count,
                    uint8_t vector_length) {
  size_t total_bits = input_length * 8;

  for (size_t i = 0; i < vector_count; i++) {
    uint64_t vector = 0;

    for (uint8_t j = 0; j < vector_length; j++) {
      size_t bit_index = i * vector_length + j;

      if (bit_index >= total_bits)
        continue;

      size_t byte_idx = bit_index / 8;
      size_t bit_off = bit_index % 8;
      uint8_t bit = (input[byte_idx] >> bit_off) & 1;

      vector |= ((uint64_t)bit << j);
    }

    bit_vecs[i] = vector;
  }
}

char *
bytes_from_vectors(const uint64_t *vecs,
                   size_t vector_count,
                   uint8_t vector_length,
                   size_t byte_count) {
  uint8_t *bytes = safe_malloc(byte_count + 1);
  memset(bytes, 0, byte_count + 1);

  size_t bit_count = byte_count * 8;

  for (size_t i = 0; i < vector_count; i++) {
    for (uint8_t j = 0; j < vector_length; j++) {
      size_t index = i * vector_length + j;

      if (index >= bit_count)
        return (char *)bytes;  // rest is padding

      bytes[index / 8] |= (uint8_t)(((vecs[i] >> j) & 1) << (index % 8));
    }
  }

  return (char *)bytes;
}
