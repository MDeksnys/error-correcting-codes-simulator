#pragma once

#include "matrix.h"

// Encodes a single message vector.
//
// matrix_p - pointer to a valid generator matrix
// vec      - message vector to encode (length must equal matrix row count)
//
// Returns a newly allocated null-terminated string representing the encoded codeword.
// Caller is responsible for freeing the returned memory.
char *
encode(Matrix *matrix_p, char *vec);

// Encodes an array of message vectors using the provided generator matrix.
//
// matrix_p  - pointer to a valid generator matrix
// vectors   - array of message vectors to encode
// vec_count - number of vectors in the array
//
// Returns a newly allocated array of codewords. Each codeword is also newly allocated.
// Caller is responsible for freeing both the array and each individual codeword.
char **
encode_all(Matrix *matrix_p, char **vectors, size_t vec_count);
