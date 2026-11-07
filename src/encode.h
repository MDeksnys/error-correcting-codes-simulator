#pragma once

#include "matrix.h"

// Encodes a single vector.
//
// matrix_p - pointer to a valid generator matrix
// vec      - vector to encode
//
// Returns the encoded vector.
uint64_t
encode(Matrix *matrix_p, uint64_t vec);

// Encodes an array of message vectors using the provided generator matrix.
//
// matrix_p  - pointer to a valid generator matrix
// vectors   - array of message vectors to encode
// vec_count - number of vectors in the array
//
// Returns a newly allocated array of codewords.
// Caller is responsible for freeing the returned array.
uint64_t *
encode_all(Matrix *matrix_p, uint64_t *vectors, size_t vec_count);
