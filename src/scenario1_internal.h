#pragma once

#include <stdint.h>

// Requests a binary input vector from the user of a specified length.
//
// length - desired vector length
//
// Returns a newly allocated null-terminated string containing the vector.
// Caller is responsible for freeing the returned memory.
static inline char *
request_input_vector(uint8_t length);


// Prints all vectors relevant to a transmission scenario.
//
// input    - original input vector
// encoded  - encoded codeword
// error    - error vector
// received - received vector
static inline void
print_all_vectors(char *input, char *encoded, char *error, char *received);
