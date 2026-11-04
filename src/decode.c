#include "decode.h"

#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "decode_internal.h"
#include "memory.h"
#include "vector.h"

char *
decode(Matrix *matrix_p, char *vec) {
  assert(vec != NULL);
  assert(matrix_is_valid(matrix_p));
  assert(strlen(vec) == matrix_p->col_count);

  char *codeword = step_by_step(matrix_p, vec);
  char *result = decode_codeword(matrix_p, codeword);
  safe_free(codeword);

  return result;
}

char **
decode_all(Matrix *matrix_p, char **vectors, size_t vec_count) {
  char **result = safe_malloc(vec_count * sizeof(char *));

  #pragma omp parallel for
  for (size_t i = 0; i < vec_count; i++)
    result[i] = decode(matrix_p, vectors[i]);

  return result;
}

static inline void
compute_syndromes_and_weights(char ***syndromes_p,
                              uint8_t **weights_p,
                              Matrix *parity_check_matrix_p,
                              size_t coset_count) {
  char **syndromes = *syndromes_p;
  uint8_t *weights = *weights_p;
  Matrix parity_check_matrix = *parity_check_matrix_p;

  uint8_t col_count = parity_check_matrix.col_count;

  uint8_t *positions = safe_malloc(col_count);
  char *work_vec = vector_zero(col_count);
  size_t unique_syndrome_count = 0;

  for (uint8_t weight = 0; weight <= col_count; weight++) {
    if (unique_syndrome_count == coset_count)
      break;

    for (uint8_t i = 0; i < weight; i++)
      positions[i] = i;

    vector_from_positions(&work_vec, positions, weight, col_count);

    do {
      char *syndrome = vector_syndrome(parity_check_matrix_p, work_vec);

      if (!syndromes_contains(syndromes, syndrome, unique_syndrome_count)) {
        syndromes[unique_syndrome_count] = syndrome;
        weights[unique_syndrome_count] = vector_weight(work_vec);
        unique_syndrome_count++;
      } else
        safe_free(syndrome);
    } while (unique_syndrome_count != coset_count &&
             next_vector_with_weight(&work_vec, &positions, weight, col_count));
  }

  safe_free(positions);
  safe_free(work_vec);
}

static inline bool
syndromes_contains(char **syndromes, char *syndrome, size_t count) {
  for (size_t i = 0; i < count; i++)
    if (strcmp(syndromes[i], syndrome) == 0)
      return true;

  return false;
}

static inline uint8_t
find_associated_weight(char *syndrome, char **syndromes, uint8_t *weights, size_t count) {
  for (size_t i = 0; i < count; i++)
    if (strcmp(syndrome, syndromes[i]) == 0)
      return weights[i];

  return UINT8_MAX;
}

static inline char *
step_by_step(Matrix *matrix_p, char *vec) {
  uint8_t col_count = matrix_p->col_count;
  uint8_t row_count = matrix_p->row_count;

  Matrix parity_check_matrix = matrix_parity_check(matrix_p);

  size_t coset_count = 1 << (col_count - row_count);
  char **syndromes = safe_malloc(coset_count * sizeof(char *));
  uint8_t *weights = safe_malloc(coset_count);
  compute_syndromes_and_weights(&syndromes, &weights, &parity_check_matrix, coset_count);

  char *work_vec = safe_strdup(vec);

  char *syndrome = vector_syndrome(&parity_check_matrix, work_vec);
  uint8_t current_weight =
    find_associated_weight(syndrome, syndromes, weights, coset_count);
  safe_free(syndrome);

  uint8_t i = 0;
  while (current_weight != 0) {
    char *temp_vec = vector_zero(col_count);
    temp_vec[i] = '1';

    char *sum = vector_add(temp_vec, work_vec, col_count);

    syndrome = vector_syndrome(&parity_check_matrix, sum);
    uint8_t temp_weight =
      find_associated_weight(syndrome, syndromes, weights, coset_count);

    if (temp_weight < current_weight) {
      safe_free(work_vec);
      work_vec = sum;
      current_weight = temp_weight;
    } else
      safe_free(sum);

    safe_free(temp_vec);
    safe_free(syndrome);

    i++;
  }

  for (size_t i = 0; i < coset_count; i++)
    safe_free(syndromes[i]);

  safe_free(syndromes);
  safe_free(weights);
  safe_free(parity_check_matrix.data);
  safe_free(parity_check_matrix.order);

  return work_vec;
}

static inline char *
decode_codeword(Matrix *matrix_p, char *vec) {
  uint8_t row_count = matrix_p->row_count;

  char *work_vec = vector_zero(row_count);
  uint8_t *positions = safe_malloc(row_count);

  for (uint8_t weight = 0; weight <= row_count; weight++) {
    for (uint8_t i = 0; i < weight; i++)
      positions[i] = i;

    vector_from_positions(&work_vec, positions, weight, row_count);

    do {
      char *product = vector_matrix_mul(work_vec, matrix_p);

      if (strcmp(product, vec) == 0) {
        safe_free(product);
        safe_free(positions);

        return work_vec;
      }

      safe_free(product);
    } while (next_vector_with_weight(&work_vec, &positions, weight, row_count));
  }

  assert(false && "unreachable: provided vec must be a valid codeword");
  return NULL;
}
