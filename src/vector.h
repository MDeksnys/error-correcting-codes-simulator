#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "matrix.h"

typedef struct {
    bool ok;
    uint64_t vec;
} ParseResult;

// Parses a string of '0' and '1' characters into a vector.
// Character i becomes coordinate i.
//
// str    - null-terminated string
// length - expected length of the vector (at most 64)
//
// Returns ok = true and the vector if the string has exactly length valid
// characters. Otherwise ok = false and vec = 0.
ParseResult vector_parse(const char *str, uint8_t length);

// Writes a vector as a null-terminated string of '0' and '1' characters,
// coordinate 0 first.
//
// vec    - the vector
// length - number of coordinates to write (at most 64)
// out    - buffer of at least length + 1 characters
void vector_format(uint64_t vec, uint8_t length, char *out);

// Computes the syndrome of a binary vector.
//
// parity_check_matrix_p - pointer to a valid parity-check matrix
// vec                   - binary vector whose syndrome is to be computed
//
// Returns the syndrome.
uint64_t vector_syndrome(Matrix *parity_check_matrix_p, uint64_t vec);

// Computes the weight of a binary vector.
//
// vec - the vector
//
// Returns the number of bits set to 1.
uint8_t vector_weight(uint64_t vec);

// Creates a vector with 1s at the given coordinates.
//
// positions - array of coordinates to set
// weight    - number of coordinates in the array
//
// Returns the vector.
uint64_t vector_from_positions(const uint8_t *positions, uint8_t weight);

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
bool next_vector_with_weight(
    uint64_t *vec_p,
    uint8_t **positions_p,
    uint8_t weight,
    uint8_t length
);
