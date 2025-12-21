#pragma once

#include "matrix.h"

// Decodes a single binary vector.
//
// matrix_p - pointer to a valid generator matrix
// vec      - null-terminated string representing the binary vector to decode
//
// Returns a newly allocated null-terminated string containing the decoded vector.
// Caller is responsible for freeing the returned memory.
char *
decode(Matrix *matrix_p, char *vec);

// Decodes an array of binary vectors.
//
// matrix_p  - pointer to a valid generator matrix
// vectors   - array of null-terminated strings representing binary vectors
// vec_count - number of vectors in the array
//
// Returns a newly allocated array of decoded vectors. Each vector is also newly allocated.
// Caller is responsible for freeing both the array and each vector.
char **
decode_all(Matrix *matrix_p, char **vectors, size_t vec_count);
