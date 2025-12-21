#pragma once

#include <stddef.h>
#include <stdint.h>

// Generates a random binary error vector of given length.
//
// error_probability - probability that a bit is an error (between 0.0 and 1.0)
// length            - length of the error vector to generate
//
// Returns a newly allocated null-terminated string representing the error vector.
// Caller is responsible for freeing the returned memory.
char *
channel_error_vector(double error_probability, uint8_t length);

// Applies random errors to an array of binary vectors.
//
// vectors           - array of binary vectors to send
// vector_count      - number of vectors in the array
// error_probability - probability that any given bit is flipped
// vector_length     - length of each vector
//
// Returns a newly allocated array of vectors with errors applied. Each vector in the
// array is also newly allocated.
// Caller is responsible for freeing both the array and each vector.
char **
channel_send_vectors(char **vectors,
                     size_t vector_count,
                     double error_probability,
                     uint8_t vector_length);
