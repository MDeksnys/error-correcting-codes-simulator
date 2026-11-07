#include "vector.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "matrix.h"

ParseResult
vector_parse(const char *str, uint8_t length) {
  uint64_t vec = 0;

  for (uint8_t i = 0; i < length; i++) {
    if (str[i] != '0' && str[i] != '1')
      return (ParseResult){.ok = false};

    vec |= (uint64_t)(str[i] - '0') << i;
  }

  if (str[length] != '\0')
    return (ParseResult){.ok = false};

  return (ParseResult){.ok = true, .vec = vec};
}

void
vector_format(uint64_t vec, uint8_t length, char *out) {
  for (uint8_t i = 0; i < length; i++)
    out[i] = '0' + ((vec >> i) & 1);

  out[length] = '\0';
}

uint64_t
vector_syndrome(Matrix *parity_check_matrix_p, uint64_t vec) {
  assert(matrix_is_valid(parity_check_matrix_p));

  return matrix_vector_mul(parity_check_matrix_p, vec);
}

uint8_t
vector_weight(uint64_t vec) {
  uint8_t result = 0;

  while (vec != 0) {
    result++;
    vec &= vec - 1;  // clears lowest bit
  }

  return result;
}

uint64_t
vector_from_positions(const uint8_t *positions, uint8_t weight) {
  uint64_t vec = 0;

  for (uint8_t i = 0; i < weight; i++)
    vec |= (uint64_t)1 << positions[i];

  return vec;
}

bool
next_vector_with_weight(uint64_t *vec_p,
                        uint8_t **positions_p,
                        uint8_t weight,
                        uint8_t length) {
  assert(vec_p != NULL);
  assert(positions_p != NULL);

  uint64_t vec = *vec_p;
  uint8_t *positions = *positions_p;

  uint8_t count = 0;
  for (uint8_t i = 0; i < weight; i++) {
    if (!((vec >> (length - 1 - i)) & 1))
      break;

    count++;
  }

  // given vector was the final vector with the desired weight
  if (count == weight)
    return false;

  uint8_t index = weight - 1 - count;
  positions[index] += 1;
  for (uint8_t i = 1; i <= count; i++)
    positions[index + i] = positions[index] + i;

  *vec_p = vector_from_positions(positions, weight);

  return true;
}
