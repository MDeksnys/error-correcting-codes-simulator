#pragma once

#include <stddef.h>
#include <stdint.h>

// Splits a string into an array of fixed-length vectors.
//
// vecs          - array of vector_count vectors to fill
// input         - the bytes to split
// input_length  - number of bytes in input
// vector_count  - number of vectors to generate
// vector_length - length of each vector (at most 64)
//
// Bit i of the stream becomes coordinate (i % vector_length) of vector
// (i / vector_length). The last vector is padded with 0s if the input does
// not fill it.
void
vectors_from_string(uint64_t *vecs,
                    const char *input,
                    size_t input_length,
                    size_t vector_count,
                    uint8_t vector_length);

// Joins an array of vectors back into a string.
//
// vecs          - array of vectors
// vector_count  - number of vectors in the array
// vector_length - length of each vector (at most 64)
// byte_count    - number of bytes to produce, any padding bits after them are
//                 ignored
//
// Returns a newly allocated buffer of byte_count + 1 bytes, null-terminated.
// Caller is responsible for freeing the returned memory.
char *
bytes_from_vectors(const uint64_t *vecs,
                   size_t vector_count,
                   uint8_t vector_length,
                   size_t byte_count);
