#pragma once

#include <stdint.h>

// Requests a binary input vector from the user of a specified length.
//
// length - desired vector length
//
// Returns the user provided vector.
static inline uint64_t request_input_vector(uint8_t length);

// Discards the rest of the current line of standard input.
static inline void discard_line(void);

// Prints all vectors relevant to a transmission scenario.
//
// k        - length of the message vector
// n        - length of the other vectors
// input    - original input vector
// encoded  - encoded codeword
// error    - error vector
// received - received vector
static inline void print_all_vectors(
    uint8_t k,
    uint8_t n,
    uint64_t input,
    uint64_t encoded,
    uint64_t error,
    uint64_t received
);
