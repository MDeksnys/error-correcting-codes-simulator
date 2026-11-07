#pragma once

#include "matrix.h"
#include <stdint.h>
#include <stddef.h>

// Decodes a single binary vector.
//
// matrix_p - pointer to a valid generator matrix
// vec      - the binary vector to decode
//
// Returns the decoded vector.
uint64_t decode(Matrix *matrix_p, uint64_t vec);

// Decodes an array of binary vectors.
//
// matrix_p  - pointer to a valid generator matrix
// vectors   - array of binary vectors
// vec_count - number of vectors in the array
//
// Returns a newly allocated array of decoded vectors.
// Caller is responsible for freeing the returned array.
uint64_t *decode_all(Matrix *matrix_p, uint64_t *vectors, size_t vec_count);
