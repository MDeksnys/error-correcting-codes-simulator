#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint8_t row_count;
  uint8_t col_count;
  uint8_t *data;
  uint8_t *order;
} Matrix;

// Constructs a Matrix struct from a string representation of its data.
//
// str       - input string containing '0' and '1' characters (commas will be ignored)
// row_count - number of rows in the matrix
// col_count - number of columns in the matrix
//
// Returns a newly allocated Matrix structure containing the parsed data.
// If the input string is invalid, returns a Matrix with all fields set to 0 / NULL.
// Caller is responsible for freeing the allocated memory in the Matrix.
Matrix
matrix_from_str(char *str, uint8_t row_count, uint8_t col_count);

// Prints the contents of a Matrix.
//
// matrix_p - pointer to a valid Matrix
void
matrix_print(Matrix *matrix_p);

// Constructs the parity-check matrix for a given generator matrix.
//
// matrix_p - pointer to a valid generator Matrix
//
// Returns a Matrix struct representing the parity-check matrix
// Caller is responsible for freeing the allocated memory in the Matrix.
Matrix
matrix_parity_check(Matrix *matrix_p);

// Multiplies a matrix by a binary vector (mod 2).
//
// matrix_p - pointer to a valid Matrix
// vec      - null-terminated binary string
//
// Returns a newly allocated null-terminated string representing the result.
// Caller is responsible for freeing the returned string.
char *
matrix_vector_mul(Matrix *matrix_p, char *vec);

// Multiplies a binary vector by a matrix (mod 2).
//
// vec      - null-terminated binary string
// matrix_p - pointer to a valid Matrix
//
// Returns a newly allocated null-terminated string representing the result.
// Caller is responsible for freeing the returned string.
char *
vector_matrix_mul(char *vec, Matrix *matrix_p);

// Converts a matrix into standard form.
//
// matrix_p - pointer to a valid Matrix
//
// Returns a newly allocated Matrix in standard form.
// Caller is responsible for freeing the allocated memory in the Matrix.
Matrix
matrix_standardized(Matrix *matrix_p);

// Checks whether the given Matrix is a valid generator matrix.
//
// matrix_p - pointer to the Matrix to validate
//
// Returns true if the matrix is valid, false otherwise.
bool
matrix_is_valid(Matrix *matrix_p);
