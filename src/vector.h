#pragma once

#include <stdbool.h>

#include "matrix.h"

// Checks whether a binary vector is valid.
//
// vec             - the string to check
// expected_length - the expected length of the vector
//
// Returns true if the string contains only '0' and '1' characters
// and is null-terminated at the expected length, otherwise false.
bool
vector_is_valid(char *vec, uint8_t expected_length);

// Computes the sum of two binary vectors.
//
// vec1, vec2 - valid binary vectors
// length     - the length of both vectors
//
// Returns a newly allocated null-terminated string containing the sum.
// Caller is responsible for freeing the returned memory.
char *
vector_add(char *vec1, char *vec2, uint8_t length);

// Creates a zero vector of specified length.
//
// length - the desired length of the vector
//
// Returns a newly allocated null-terminated string consisting of all '0' characters.
// Caller is responsible for freeing the returned memory.
char *
vector_zero(uint8_t length);

// Computes the syndrome of a binary vector.
//
// parity_check_matrix_p - pointer to a valid parity-check matrix
// vec                   - binary vector whose syndrome is to be computed
//
// Returns a newly allocated null-terminated string representing the syndrome.
// Caller is responsible for freeing the returned memory.
char *
vector_syndrome(Matrix *parity_check_matrix_p, char *vec);

// Computes the weight of a binary vector.
//
// vec - null-terminated binary vector
//
// Returns the number of bits set to '1'.
uint8_t
vector_weight(char *vec);

// Fills a binary vector based on positions of '1's.
//
// vec_p     - pointer to a binary vector to be filled
// positions - array of positions to set to '1'
// weight    - number of positions to set
// length    - total length of the vector
//
// The vector is null-terminated. All other positions are set to '0'.
void
vector_from_positions(char **vec_p, uint8_t *positions, uint8_t weight, uint8_t length);

// Advances a binary vector to the next vector of the same weight.
//
// vec_p       - pointer to the current binary vector
// positions_p - pointer to the array of positions of '1's in the vector
// weight      - weight of the vector
// length      - total length of the vector
//
// Returns false if the given vector was the last one with the given weight,
// true otherwise.
//
// Example:
//   The first few vectors of length 5 and weight 3,
//   in the order produced by next_vector_with_weight:
//     Vector  Positions
//     ------  ---------
//     11100    [0, 1, 2]
//     11010    [0, 1, 3]
//     11001    [0, 1, 4]
//     10110    [0, 2, 3]
//     10101    [0, 2, 4]
//     10011    [0, 3, 4]
//     01110    [1, 2, 3]
//     ...
bool
next_vector_with_weight(char **vec_p,
                        uint8_t **positions_p,
                        uint8_t weight,
                        uint8_t length);
