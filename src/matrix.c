#include "matrix.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "matrix_internal.h"
#include "memory.h"

Matrix matrix_from_str(const char *str, uint8_t row_count, uint8_t col_count) {
    if (str == NULL || row_count == 0 || col_count == 0)
        return (Matrix){0};

    // row_count - 1 for each comma
    const size_t expected_len = row_count * col_count + (row_count - 1);

    if (strlen(str) != expected_len)
        return (Matrix){0};

    uint64_t *rows = safe_malloc(row_count * sizeof(*rows));
    uint64_t *cols = safe_malloc(col_count * sizeof(*cols));
    uint8_t *order = safe_malloc(col_count);

    memset(rows, 0, row_count * sizeof(*rows));

    for (size_t i = 0; i < col_count; i++)
        order[i] = i;

    const char *itr = str;

    for (size_t row = 0; row < row_count; row++) {
        for (size_t col = 0; col < col_count; col++, itr++) {
            const char symbol = *itr;
            if (symbol != '0' && symbol != '1')
                goto bad;

            if (symbol == '1')
                rows[row] |= ((uint64_t)1 << col);
        }

        if (row + 1 < row_count && *(itr++) != ',')
            goto bad;
    }

    Matrix matrix = {
        .row_count = row_count,
        .col_count = col_count,
        .rows = rows,
        .cols = cols,
        .order = order
    };

    matrix_rebuild_cols(&matrix);

    return matrix;

bad:
    safe_free(order);
    safe_free(rows);
    safe_free(cols);
    return (Matrix){0};
}

Matrix matrix_parity_check(Matrix *matrix_p) {
    assert(matrix_p != NULL);
    assert(matrix_p->rows);
    assert(matrix_p->cols != NULL);
    assert(matrix_p->order != NULL);

    Matrix g = *matrix_p;
    Matrix g_std_form = matrix_standardized(matrix_p);

    Matrix h = (Matrix){
        .row_count = g.col_count - g.row_count,
        .col_count = g.col_count,
        .rows = safe_malloc((g.col_count - g.row_count) * sizeof(uint64_t)),
        .cols = safe_malloc(g.col_count * sizeof(uint64_t)),
        .order = safe_malloc(g.col_count)
    };

    memset(h.rows, 0, h.row_count * sizeof(uint64_t));

    for (size_t i = 0; i < h.col_count; i++)
        h.order[i] = g_std_form.order[i];

    // H = (-A^T|I)
    // below code adds the A part
    // the '-' is ignore because mod2
    for (size_t i = 0; i < h.row_count; i++) {
        size_t base = i + g_std_form.row_count;
        h.rows[i] = g_std_form.cols[base];
    }

    // H = (-A^T|I)
    // below code adds the I part
    size_t offset = h.col_count - h.row_count;
    for (size_t row = 0; row < h.row_count; row++)
        h.rows[row] |= ((uint64_t)1 << (offset + row));

    matrix_rebuild_cols(&h);
    matrix_default_order(&h);

    safe_free(g_std_form.order);
    safe_free(g_std_form.rows);
    safe_free(g_std_form.cols);

    return h;
}

uint64_t matrix_vector_mul(Matrix *matrix_p, uint64_t vec) {
    assert(matrix_p != NULL);
    assert(matrix_p->rows);
    assert(matrix_p->cols != NULL);
    assert(matrix_p->order != NULL);

    uint8_t col_count = matrix_p->col_count;
    uint64_t result = 0;

    // using XOR because mod2
    for (size_t col = 0; col < col_count; col++)
        if ((vec >> col) & 1)
            result ^= matrix_p->cols[col];

    return result;
}

uint64_t vector_matrix_mul(uint64_t vec, Matrix *matrix_p) {
    assert(matrix_p != NULL);
    assert(matrix_p->rows);
    assert(matrix_p->cols != NULL);
    assert(matrix_p->order != NULL);

    uint8_t row_count = matrix_p->row_count;
    uint64_t result = 0;

    // using XOR because mod2
    for (size_t row = 0; row < row_count; row++)
        if ((vec >> row) & 1)
            result ^= matrix_p->rows[row];

    return result;
}

Matrix matrix_standardized(Matrix *matrix_p) {
    assert(matrix_p != NULL);
    assert(matrix_p->rows);
    assert(matrix_p->cols != NULL);
    assert(matrix_p->order != NULL);

    Matrix new_matrix = matrix_clone(matrix_p);
    uint8_t row_count = new_matrix.row_count;

    for (size_t row = 0; row < row_count; row++) {
        if (!((new_matrix.rows[row] >> row) & 1))
            try_swap_rows(&new_matrix, row);

        if (!((new_matrix.rows[row] >> row) & 1))
            try_swap_cols(&new_matrix, row);

        try_add_row_to_every_other(&new_matrix, row);
    }

    return new_matrix;
}

bool matrix_is_valid(Matrix *matrix_p) {
    if (matrix_p == NULL ||
        matrix_p->rows == NULL ||
        matrix_p->cols == NULL ||
        matrix_p->order == NULL)
        return false;

    bool result = true;
    Matrix temp = matrix_standardized(matrix_p);
    uint8_t row_count = temp.row_count;

    for (size_t row = 0; row < row_count; row++) {
        if (temp.rows[row] == 0) {
            result = false;
            break;
        }
    }

    safe_free(temp.order);
    safe_free(temp.rows);
    safe_free(temp.cols);

    return result;
}

static inline void matrix_swap_rows(Matrix *matrix_p, uint8_t r1, uint8_t r2) {
    assert(matrix_p != NULL);
    assert(matrix_p->rows);
    assert(matrix_p->cols != NULL);
    assert(matrix_p->order != NULL);
    assert(r1 != r2);
    assert(r1 < matrix_p->row_count);
    assert(r2 < matrix_p->row_count);

    // using this property of XOR: a^b^b == a
    matrix_p->rows[r1] ^= matrix_p->rows[r2];
    matrix_p->rows[r2] ^= matrix_p->rows[r1];
    matrix_p->rows[r1] ^= matrix_p->rows[r2];

    matrix_rebuild_cols(matrix_p);
}

static inline void matrix_swap_cols(Matrix *matrix_p, uint8_t c1, uint8_t c2) {
    assert(matrix_p != NULL);
    assert(matrix_p->rows);
    assert(matrix_p->cols != NULL);
    assert(matrix_p->order != NULL);
    assert(c1 != c2);
    assert(c1 < matrix_p->col_count);
    assert(c2 < matrix_p->col_count);

    uint8_t *order = matrix_p->order;

    // Using this property of XOR: a^b^b == a

    order[c1] ^= order[c2];
    order[c2] ^= order[c1];
    order[c1] ^= order[c2];

    matrix_p->cols[c1] ^= matrix_p->cols[c2];
    matrix_p->cols[c2] ^= matrix_p->cols[c1];
    matrix_p->cols[c1] ^= matrix_p->cols[c2];

    for (size_t row = 0; row < matrix_p->row_count; row++) {
        uint64_t diff = ((matrix_p->rows[row] >> c1) ^ (matrix_p->rows[row] >> c2)) & 1;
        matrix_p->rows[row] ^= (diff << c1) | (diff << c2);
    }
}

static inline void matrix_default_order(Matrix *matrix_p) {
    assert(matrix_p != NULL);
    assert(matrix_p->rows);
    assert(matrix_p->cols != NULL);
    assert(matrix_p->order != NULL);

    uint8_t col_count = matrix_p->col_count;
    uint8_t *order = matrix_p->order;

    for (size_t i = 0; i < col_count; i++)
        if (order[i] != i)
            for (size_t j = i + 1; j < col_count; j++)
                if (order[j] == i)
                    matrix_swap_cols(matrix_p, i, j);
}

static inline Matrix matrix_clone(Matrix *matrix_p) {
    assert(matrix_p != NULL);
    assert(matrix_p->rows);
    assert(matrix_p->cols != NULL);
    assert(matrix_p->order != NULL);

    uint8_t row_count = matrix_p->row_count;
    uint8_t col_count = matrix_p->col_count;
    uint8_t *order = matrix_p->order;

    Matrix new_matrix = (Matrix){
        .row_count = row_count,
        .col_count = col_count,
        .rows = safe_malloc(row_count * sizeof(uint64_t)),
        .cols = safe_malloc(col_count * sizeof(uint64_t)),
        .order = safe_malloc(col_count)
    };

    for (size_t i = 0; i < row_count; i++)
        new_matrix.rows[i] = matrix_p->rows[i];

    for (size_t i = 0; i < col_count; i++) {
        new_matrix.cols[i] = matrix_p->cols[i];
        new_matrix.order[i] = order[i];
    }

    return new_matrix;
}

static inline void try_add_row_to_every_other(Matrix *matrix_p, uint8_t src_row_col) {
    assert(matrix_p != NULL);
    assert(matrix_p->rows);
    assert(matrix_p->cols != NULL);
    assert(matrix_p->order != NULL);
    assert(src_row_col < matrix_p->row_count);
    assert(src_row_col < matrix_p->col_count);

    uint8_t row_count = matrix_p->row_count;
    uint64_t src_val = matrix_p->rows[src_row_col];

    for (size_t row = 0; row < row_count; row++)
        if (row != src_row_col && ((matrix_p->rows[row] >> src_row_col) & 1) == 1)
            matrix_p->rows[row] ^= src_val;

    matrix_rebuild_cols(matrix_p);
}

static inline void try_swap_rows(Matrix *matrix_p, uint8_t target_row_col) {
    assert(matrix_p != NULL);
    assert(matrix_p->rows);
    assert(matrix_p->cols != NULL);
    assert(matrix_p->order != NULL);

    uint8_t row_count = matrix_p->row_count;

    for (size_t row = target_row_col + 1; row < row_count; row++) {
        if (((matrix_p->rows[row] >> target_row_col) & 1) == 1) {
            matrix_swap_rows(matrix_p, target_row_col, row);
            return;
        }
    }
}

static inline void try_swap_cols(Matrix *matrix_p, uint8_t target_row_col) {
    assert(matrix_p != NULL);
    assert(matrix_p->rows);
    assert(matrix_p->cols != NULL);
    assert(matrix_p->order != NULL);

    uint8_t col_count = matrix_p->col_count;

    for (size_t col = target_row_col + 1; col < col_count; col++) {
        if (((matrix_p->rows[target_row_col] >> col) & 1) == 1) {
            matrix_swap_cols(matrix_p, target_row_col, col);
            return;
        }
    }
}

static inline void matrix_rebuild_cols(Matrix *matrix_p) {
    memset(matrix_p->cols, 0, matrix_p->col_count * sizeof(uint64_t));

    for (size_t row = 0; row < matrix_p->row_count; row++)
        for (size_t col = 0; col < matrix_p->col_count; col++)
            if ((matrix_p->rows[row] >> col) & 1)
                matrix_p->cols[col] |= ((uint64_t)1 << row);
}
