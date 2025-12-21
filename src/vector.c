#include "vector.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "matrix.h"
#include "memory.h"

bool
vector_is_valid(char *vec, uint8_t expected_length) {
  assert(vec != NULL);

  for (uint8_t i = 0; i < expected_length; i++) {
    char c = vec[i];
    if (c != '0' && c != '1')
      return false;
  }

  if (vec[expected_length] != '\0')
    return false;

  return true;
}

char *
vector_add(char *vec1, char *vec2, uint8_t length) {
  assert(vector_is_valid(vec1, length));
  assert(vector_is_valid(vec2, length));

  char *sum = safe_malloc(length + 1);

  for (uint8_t i = 0; i < length; i++)
    sum[i] = '0' + (vec1[i] ^ vec2[i]);
  sum[length] = '\0';

  return sum;
}

char *
vector_zero(uint8_t length) {
  char *vec = safe_malloc(length + 1);

  for (uint8_t i = 0; i < length; i++)
    vec[i] = '0';
  vec[length] = '\0';

  return vec;
}

char *
vector_syndrome(Matrix *parity_check_matrix_p, char *vec) {
  assert(vec != NULL);
  assert(matrix_is_valid(parity_check_matrix_p));
  assert(strlen(vec) == parity_check_matrix_p->col_count);

  return matrix_vector_mul(parity_check_matrix_p, vec);
}

uint8_t
vector_weight(char *vec) {
  assert(vec != NULL);

  uint8_t result = 0;

  for (char *ptr = vec; *ptr != '\0'; ptr++)
    result += *ptr != '0' ? 1 : 0;

  return result;
}

void
vector_from_positions(char **vec_p, uint8_t *positions, uint8_t weight, uint8_t length) {
  assert(vec_p != NULL);
  assert(*vec_p != NULL);

  char *vec = *vec_p;

  for (uint8_t i = 0; i < length; i++)
    vec[i] = '0';

  for (uint8_t i = 0; i < weight; i++)
    vec[positions[i]] = '1';

  vec[length] = '\0';
}

bool
next_vector_with_weight(char **vec_p,
                        uint8_t **positions_p,
                        uint8_t weight,
                        uint8_t length) {
  assert(vec_p != NULL);
  assert(*vec_p != NULL);
  assert(positions_p != NULL);

  char *vec = *vec_p;
  uint8_t *positions = *positions_p;

  uint8_t count = 0;
  for (uint8_t i = 0; i < weight; i++) {
    if (vec[length - 1 - i] != '1')
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

  vector_from_positions(vec_p, positions, weight, length);

  return true;
}
