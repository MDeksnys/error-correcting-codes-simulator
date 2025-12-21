#pragma once

#include "matrix.h"

// Splits a bit string into an array of fixed-length binary vectors.
//
// vectors_p     - pointer to an array of char * to hold the resulting vectors
// input         - the input bit string
// input_length  - length of the input string
// vector_count  - number of vectors to generate
// vector_length - length of each vector
//
// Returns the number of '0' bits appended to pad the final vector.
// Caller is responsible for freeing the returned vectors.
uint8_t
vectors_from_bit_string(char ***vectors_p,
                        char *input,
                        size_t input_length,
                        size_t vector_count,
                        uint8_t vector_length);

// Concatenates an array of binary vectors back into a single bit string.
//
// vectors        - array of binary vectors
// count_appended - number of padded bits that should be ignored at the end
// vector_count   - number of vectors in the array
// vector_length  - length of each vector
//
// Returns a newly allocated null-terminated string representing the concatenated bits.
// Caller is responsible for freeing the returned memory.
char *
bit_string_from_vectors(char **vectors,
                        uint8_t count_appended,
                        size_t vector_count,
                        uint8_t vector_length);

// Creates a string of bytes from a string of bits (e.g., "01000001" -> "A").
//
// bit_str - null-terminated string whose length must be a multiple of 8
//
// Returns a newly allocated string of bytes corresponding to each group of 8 bits.
// Caller is responsible for freeing the returned memory.
char *
bytes_from_bit_string(char *bit_str);

// Creates a string of bits from a string of bytes (e.g., "A" -> "01000001").
//
// str    - pointer to the byte string
// length - length of the input string
//
// Returns a newly allocated null-terminated string representing the bits of each byte.
// Caller is responsible for freeing the returned memory.
char *
bit_string_from_bytes(char *str, size_t length);
