#include "decode.h"

#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "decode_internal.h"
#include "matrix.h"
#include "memory.h"
#include "vector.h"

uint64_t
decode(Matrix *matrix_p, uint64_t vec) {
  assert(matrix_is_valid(matrix_p));

  uint64_t codeword = step_by_step(matrix_p, vec);
  uint64_t result = decode_codeword(matrix_p, codeword);

  return result;
}

uint64_t *
decode_all(Matrix *matrix_p, uint64_t *vectors, size_t vec_count) {
  uint64_t *result = safe_malloc(vec_count * sizeof(uint64_t));

  #pragma omp parallel for
  for (size_t i = 0; i < vec_count; i++)
    result[i] = decode(matrix_p, vectors[i]);

  return result;
}

static inline void
compute_syndromes_and_weights(uint64_t **syndromes_p,
                              uint8_t **weights_p,
                              Matrix *parity_check_matrix_p,
                              size_t coset_count) {
  uint64_t *syndromes = *syndromes_p;
  uint8_t *weights = *weights_p;
  Matrix parity_check_matrix = *parity_check_matrix_p;

  uint8_t col_count = parity_check_matrix.col_count;

  uint8_t *positions = safe_malloc(col_count);
  uint64_t work_vec = 0;
  size_t unique_syndrome_count = 0;

  for (uint8_t weight = 0; weight <= col_count; weight++) {
    if (unique_syndrome_count == coset_count)
      break;

    for (uint8_t i = 0; i < weight; i++)
      positions[i] = i;

    work_vec = vector_from_positions(positions, weight);

    do {
      uint64_t syndrome = vector_syndrome(parity_check_matrix_p, work_vec);

      if (!syndromes_contains(syndromes, syndrome, unique_syndrome_count)) {
        syndromes[unique_syndrome_count] = syndrome;
        weights[unique_syndrome_count] = vector_weight(work_vec);
        unique_syndrome_count++;
      }
    } while (unique_syndrome_count != coset_count &&
             next_vector_with_weight(&work_vec, &positions, weight, col_count));
  }

  safe_free(positions);
}

static inline bool
syndromes_contains(uint64_t *syndromes, uint64_t syndrome, size_t count) {
  for (size_t i = 0; i < count; i++)
    if (syndromes[i] == syndrome)
      return true;

  return false;
}

static inline uint8_t
find_associated_weight(uint64_t syndrome, uint64_t *syndromes, uint8_t *weights, size_t count) {
  for (size_t i = 0; i < count; i++)
    if (syndrome == syndromes[i])
      return weights[i];

  return UINT8_MAX;
}

static inline uint64_t
step_by_step(Matrix *matrix_p, uint64_t vec) {
  uint8_t col_count = matrix_p->col_count;
  uint8_t row_count = matrix_p->row_count;

  Matrix parity_check_matrix = matrix_parity_check(matrix_p);

  size_t coset_count = (size_t)1 << (col_count - row_count);
  uint64_t *syndromes = safe_malloc(coset_count * sizeof(uint64_t));
  uint8_t *weights = safe_malloc(coset_count);
  compute_syndromes_and_weights(&syndromes, &weights, &parity_check_matrix, coset_count);

  uint64_t work_vec = vec;

  uint64_t syndrome = vector_syndrome(&parity_check_matrix, work_vec);
  uint8_t current_weight =
    find_associated_weight(syndrome, syndromes, weights, coset_count);

  uint8_t i = 0;
  while (current_weight != 0) {
    uint64_t temp_vec = 0;
    temp_vec |= (uint64_t)1 << i;

    uint64_t sum = temp_vec ^ work_vec;

    syndrome = vector_syndrome(&parity_check_matrix, sum);
    uint8_t temp_weight =
      find_associated_weight(syndrome, syndromes, weights, coset_count);

    if (temp_weight < current_weight) {
      work_vec = sum;
      current_weight = temp_weight;
    }

    i++;
  }

  safe_free(syndromes);
  safe_free(weights);
  safe_free(parity_check_matrix.data);
  safe_free(parity_check_matrix.order);

  return work_vec;
}

static inline uint64_t
decode_codeword(Matrix *matrix_p, uint64_t vec) {
  uint8_t row_count = matrix_p->row_count;

  uint64_t work_vec = 0;
  uint8_t *positions = safe_malloc(row_count);

  for (uint8_t weight = 0; weight <= row_count; weight++) {
    for (uint8_t i = 0; i < weight; i++)
      positions[i] = i;

    work_vec = vector_from_positions(positions, weight);

    do {
      uint64_t product = vector_matrix_mul(work_vec, matrix_p);

      if (product == vec) {
        safe_free(positions);
        return work_vec;
      }

    } while (next_vector_with_weight(&work_vec, &positions, weight, row_count));
  }

  assert(false && "unreachable: provided vec must be a valid codeword");
  return 0;
}
