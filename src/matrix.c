#include "matrix.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "matrix_internal.h"
#include "memory.h"

Matrix
matrix_from_str(char *str, uint8_t row_count, uint8_t col_count) {
  uint8_t *data = NULL;
  uint8_t *order = NULL;
  char *temp_str = NULL;

  temp_str = safe_strdup(str);
  remove_commas(temp_str);

  if (strlen(temp_str) != row_count * col_count)
    goto bad;

  data = safe_malloc(row_count * col_count);
  order = safe_malloc(col_count);

  for (uint8_t i = 0; i < col_count; i++)
    order[i] = i;

  for (uint8_t row = 0; row < row_count; row++) {
    for (uint8_t col = 0; col < col_count; col++) {
      uint8_t index = row * col_count + col;

      uint8_t value = temp_str[index] - '0';
      if (value != 0 && value != 1)
        goto bad;

      data[index] = value;
    }
  }

  safe_free(temp_str);
  return (
    Matrix){.row_count = row_count, .col_count = col_count, .data = data, .order = order};

bad:
  safe_free(order);
  safe_free(data);
  safe_free(temp_str);
  return (Matrix){0};
}

void
matrix_print(Matrix *matrix_p) {
  assert(matrix_p != NULL && matrix_p->data != NULL && matrix_p->order != NULL);

  uint8_t row_count = matrix_p->row_count;
  uint8_t col_count = matrix_p->col_count;
  uint8_t *data = matrix_p->data;

  for (uint8_t row = 0; row < row_count; row++) {
    for (uint8_t col = 0; col < col_count; col++)
      printf("%d", data[row * col_count + col]);
    printf("\n");
  }
}

Matrix
matrix_parity_check(Matrix *matrix_p) {
  assert(matrix_p != NULL && matrix_p->data != NULL && matrix_p->order != NULL);

  Matrix g = *matrix_p;
  Matrix g_std_form = matrix_standardized(matrix_p);

  Matrix h = (Matrix){.row_count = g.col_count - g.row_count,
                      .col_count = g.col_count,
                      .data = safe_malloc((g.col_count - g.row_count) * g.col_count),
                      .order = safe_malloc(g.col_count)};

  for (uint8_t i = 0; i < h.col_count; i++)
    h.order[i] = g_std_form.order[i];

  // H = (-A^T|I)
  // below code adds the A part
  // the '-' is ignore because mod2
  for (uint8_t i = 0; i < h.row_count; i++) {
    uint8_t base1 = i * h.col_count;
    uint8_t base2 = i + g_std_form.row_count;

    for (uint8_t j = 0; j < g_std_form.row_count; j++)
      h.data[base1 + j] = g_std_form.data[base2 + j * g_std_form.col_count];
  }

  // H = (-A^T|I)
  // below code adds the I part
  for (uint8_t row = 0; row < h.row_count; row++) {
    uint8_t base = row * h.col_count;
    uint8_t offset = h.col_count - h.row_count;

    for (uint8_t col = 0; col + offset < h.col_count; col++)
      h.data[base + col + offset] = row == col ? 1 : 0;
  }

  matrix_default_order(&h);

  safe_free(g_std_form.order);
  safe_free(g_std_form.data);

  return h;
}

uint64_t
matrix_vector_mul(Matrix *matrix_p, uint64_t vec) {
  assert(matrix_p != NULL && matrix_p->data != NULL && matrix_p->order != NULL);

  uint8_t col_count = matrix_p->col_count;
  uint8_t row_count = matrix_p->row_count;
  uint8_t *data = matrix_p->data;

  uint64_t result = 0;

  // using XOR because mod2
  for (uint8_t col = 0; col < col_count; col++)
    if ((vec >> col) & 1)
      for (uint8_t row = 0; row < row_count; row++)
        result ^= (uint64_t)data[row * col_count + col] << row;

  return result;
}

uint64_t
vector_matrix_mul(uint64_t vec, Matrix *matrix_p) {
  assert(matrix_p != NULL && matrix_p->data != NULL && matrix_p->order != NULL);

  uint8_t col_count = matrix_p->col_count;
  uint8_t row_count = matrix_p->row_count;
  uint8_t *data = matrix_p->data;

  uint64_t result = 0;

  // using XOR because mod2
  for (uint8_t row = 0; row < row_count; row++)
    if ((vec >> row) & 1)
      for (uint8_t col = 0; col < col_count; col++)
        result ^= (uint64_t)data[row * col_count + col] << col;

  return result;
}

Matrix
matrix_standardized(Matrix *matrix_p) {
  assert(matrix_p != NULL && matrix_p->data != NULL && matrix_p->order != NULL);

  Matrix new_matrix = matrix_clone(matrix_p);

  uint8_t row_count = new_matrix.row_count;
  uint8_t col_count = new_matrix.col_count;
  uint8_t *data = new_matrix.data;

  for (uint8_t row = 0; row < row_count; row++) {
    uint8_t base = row * col_count;

    if (data[base + row] != 1)
      try_swap_rows(&new_matrix, row);

    if (data[base + row] != 1)
      try_swap_cols(&new_matrix, row);

    try_add_row_to_every_other(&new_matrix, row);
  }

  return new_matrix;
}

bool
matrix_is_valid(Matrix *matrix_p) {
  if (matrix_p == NULL || matrix_p->data == NULL || matrix_p->order == NULL)
    return false;

  bool result = true;
  Matrix temp = matrix_standardized(matrix_p);

  uint8_t row_count = temp.row_count;
  uint8_t col_count = temp.col_count;
  uint8_t *data = temp.data;

  for (uint8_t row = 0; row < row_count; row++) {
    uint8_t count_of_ones = 0;

    for (uint8_t col = 0; col < col_count; col++) {
      uint8_t value = data[row * col_count + col];

      switch (value) {
        case 0:
          break;
        case 1:
          count_of_ones++;
          break;
        default:
          return false;
      }
    }

    if (count_of_ones == 0) {
      result = false;
      break;
    }
  }

  safe_free(temp.data);
  safe_free(temp.order);

  return result;
}

static inline void
matrix_swap_rows(Matrix *matrix_p, uint8_t r1, uint8_t r2) {
  assert(matrix_p != NULL && matrix_p->data != NULL && matrix_p->order != NULL);
  assert(r1 != r2);
  assert(r1 < matrix_p->row_count);
  assert(r2 < matrix_p->row_count);

  uint8_t col_count = matrix_p->col_count;
  uint8_t *data = matrix_p->data;

  uint8_t base1 = r1 * col_count;
  uint8_t base2 = r2 * col_count;

  // using this property of XOR: a^b^b == a

  for (uint8_t col = 0; col < col_count; col++)
    data[base1 + col] ^= data[base2 + col];

  for (uint8_t col = 0; col < col_count; col++)
    data[base2 + col] ^= data[base1 + col];

  for (uint8_t col = 0; col < col_count; col++)
    data[base1 + col] ^= data[base2 + col];
}

static inline void
matrix_swap_cols(Matrix *matrix_p, uint8_t c1, uint8_t c2) {
  assert(matrix_p != NULL && matrix_p->data != NULL && matrix_p->order != NULL);
  assert(c1 != c2);
  assert(c1 < matrix_p->col_count);
  assert(c2 < matrix_p->col_count);

  uint8_t col_count = matrix_p->col_count;
  uint8_t row_count = matrix_p->row_count;
  uint8_t *data = matrix_p->data;
  uint8_t *order = matrix_p->order;

  // Using this property of XOR: a^b^b == a

  order[c1] ^= order[c2];
  order[c2] ^= order[c1];
  order[c1] ^= order[c2];

  for (uint8_t row = 0; row < row_count; row++)
    data[c1 + row * col_count] ^= data[c2 + row * col_count];

  for (uint8_t row = 0; row < row_count; row++)
    data[c2 + row * col_count] ^= data[c1 + row * col_count];

  for (uint8_t row = 0; row < row_count; row++)
    data[c1 + row * col_count] ^= data[c2 + row * col_count];
}

static inline void
matrix_default_order(Matrix *matrix_p) {
  assert(matrix_p != NULL && matrix_p->data != NULL && matrix_p->order != NULL);

  uint8_t col_count = matrix_p->col_count;
  uint8_t *order = matrix_p->order;

  for (uint8_t i = 0; i < col_count; i++)
    if (order[i] != i)
      for (uint8_t j = i + 1; j < col_count; j++)
        if (order[j] == i)
          matrix_swap_cols(matrix_p, i, j);
}

static inline Matrix
matrix_clone(Matrix *matrix_p) {
  assert(matrix_p != NULL && matrix_p->data != NULL && matrix_p->order != NULL);

  uint8_t row_count = matrix_p->row_count;
  uint8_t col_count = matrix_p->col_count;
  uint8_t *data = matrix_p->data;
  uint8_t *order = matrix_p->order;

  Matrix new_matrix = (Matrix){.row_count = row_count,
                               .col_count = col_count,
                               .data = safe_malloc(row_count * col_count),
                               .order = safe_malloc(col_count)};

  for (uint8_t i = 0; i < row_count * col_count; i++)
    new_matrix.data[i] = data[i];

  for (uint8_t i = 0; i < col_count; i++)
    new_matrix.order[i] = order[i];

  return new_matrix;
}

static inline void
try_add_row_to_every_other(Matrix *matrix_p, uint8_t src_row_col) {
  assert(matrix_p != NULL && matrix_p->data != NULL && matrix_p->order != NULL);
  assert(src_row_col < matrix_p->row_count);
  assert(src_row_col < matrix_p->col_count);

  uint8_t row_count = matrix_p->row_count;
  uint8_t col_count = matrix_p->col_count;
  uint8_t *data = matrix_p->data;

  uint8_t src_row = src_row_col;
  uint8_t src_col = src_row_col;
  uint8_t src_base = src_row * col_count;

  for (uint8_t row = 0; row < row_count; row++)
    if (row != src_row && data[row * col_count + src_col] == 1)
      for (uint8_t col = 0; col < col_count; col++)
        data[row * col_count + col] ^= data[src_base + col];
}

static inline void
try_swap_rows(Matrix *matrix_p, uint8_t target_row_col) {
  assert(matrix_p != NULL && matrix_p->data != NULL && matrix_p->order != NULL);

  uint8_t row_count = matrix_p->row_count;
  uint8_t col_count = matrix_p->col_count;
  uint8_t *data = matrix_p->data;

  for (uint8_t row = target_row_col + 1; row < row_count; row++) {
    if (data[row * col_count + target_row_col] == 1) {
      matrix_swap_rows(matrix_p, target_row_col, row);
      return;
    }
  }
}

static inline void
try_swap_cols(Matrix *matrix_p, uint8_t target_row_col) {
  assert(matrix_p != NULL && matrix_p->data != NULL && matrix_p->order != NULL);

  uint8_t col_count = matrix_p->col_count;
  uint8_t *data = matrix_p->data;

  for (uint8_t col = target_row_col + 1; col < col_count; col++) {
    if (data[col + target_row_col * col_count] == 1) {
      matrix_swap_cols(matrix_p, target_row_col, col);
      return;
    }
  }
}

static inline void
remove_commas(char *str) {
  char *write_ptr = str;
  char *read_ptr = str;

  while (*read_ptr != '\0') {
    *write_ptr = *read_ptr;

    if (*read_ptr != ',')
      write_ptr++;

    read_ptr++;
  }

  *write_ptr = '\0';
}
