#pragma once

#include <stddef.h>
#include <stdint.h>

// Generates a random binary error vector of given length.
//
// error_probability - probability that a bit is an error (between 0.0 and 1.0)
// length            - length of the error vector to generate
//
// Returns the error vector.
uint64_t
channel_error_vector(double error_probability, uint8_t length);

// Applies random errors to an array of binary vectors.
//
// vectors           - array of binary vectors to send
// vector_count      - number of vectors in the array
// error_probability - probability that any given bit is flipped
// vector_length     - length of each vector
//
// Returns a newly allocated array of vectors with errors applied.
// Caller is responsible for freeing the returned array.
uint64_t *
channel_send_vectors(uint64_t *vectors,
                     size_t vector_count,
                     double error_probability,
                     uint8_t vector_length);
