#pragma once

#include "matrix.h"

// Swaps two rows in a matrix in-place.
//
// matrix_p - pointer to the matrix
// r1, r2   - indices of the rows to swap
static inline void
matrix_swap_rows(Matrix *matrix_p, uint8_t r1, uint8_t r2);

// Swaps two columns in a matrix in-place and updates the order array.
//
// matrix_p - pointer to the matrix
// c1, c2   - indices of the columns to swap
static inline void
matrix_swap_cols(Matrix *matrix_p, uint8_t c1, uint8_t c2);

// Restores the column order of a matrix to the default (012..col_count-1).
//
// matrix_p - pointer to the matrix
static inline void
matrix_default_order(Matrix *matrix_p);

// Creates a deep copy of a matrix.
//
// matrix_p - pointer to the matrix to clone
//
// Returns a new Matrix instance with allocated data and order arrays.
// Caller is responsible for freeing the data and order in the matrix.
static inline Matrix
matrix_clone(Matrix *matrix_p);

// Adds the specified row to all other rows in the matrix where the corresponding
// column entry is 1.
//
// matrix_p      - pointer to the matrix
// src_row_col   - the row AND column index used as the source
static inline void
try_add_row_to_every_other(Matrix *matrix_p, uint8_t src_row_col);

// Attempts to swap the target row with a lower row that has a 1 in the target
// column.
//
// matrix_p         - pointer to the matrix
// target_row_col   - the target row AND target column
static inline void
try_swap_rows(Matrix *matrix_p, uint8_t target_row_col);

// Attempts to swap the target column with a right-hand column that has a 1 in
// the target row.
//
// matrix_p         - pointer to the matrix
// target_row_col   - the target row AND target column
static inline void
try_swap_cols(Matrix *matrix_p, uint8_t target_row_col);

// Removes all commas from a string in-place.
//
// str - the string to process
static inline void
remove_commas(char *str);
