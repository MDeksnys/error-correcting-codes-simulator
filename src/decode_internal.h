#pragma once

#include "matrix.h"
#include <stdint.h>
#include <stddef.h>

// Computes all unique syndromes and their associated weights for a given
// parity-check matrix.
//
// syndromes_p            - pointer to an array of syndromes
// weights_p              - pointer to an array to store weights corresponding to
//                          each syndrome
// parity_check_matrix_p  - pointer to the parity-check matrix
// coset_count            - total number of cosets (the number of unique syndromes)
//
// Populates the provided arrays with unique syndromes and their associated weights.
static inline void compute_syndromes_and_weights(
    uint64_t **syndromes_p,
    uint8_t **weights_p,
    Matrix *parity_check_matrix_p,
    size_t coset_count
);

// Checks if a syndrome is present in the array of syndromes.
//
// syndromes - array of syndromes
// syndrome  - syndrome to search for
// count     - number of syndromes currently in the array
//
// Returns true if the syndrome is in the array, false otherwise.
static inline bool syndromes_contains(
    uint64_t *syndromes,
    uint64_t syndrome,
    size_t count
);

// Finds the weight associated with a given syndrome.
//
// syndrome  - syndrome to search for
// syndromes - array of syndromes
// weights   - array of weights corresponding to syndromes
// count     - number of syndromes in the array
//
// Returns the weight of the associated syndrome, or UINT8_MAX if
// not found (unreachable if syndromes and weights are correct).
static inline uint8_t find_associated_weight(
    uint64_t syndrome,
    uint64_t *syndromes,
    uint8_t *weights,
    size_t count
);

// Corrects errors in a received vector using step-by-step decoding algorithm.
//
// matrix_p - pointer to a valid generator matrix
// vec      - binary vector to correct
//
// Returns the corrected vector, a valid codeword for the code generated with matrix_p.
static inline uint64_t step_by_step(Matrix *matrix_p, uint64_t vec);

// Decodes a corrected codeword vector into its original message.
//
// matrix_p - pointer to a valid generator matrix
// vec      - corrected codeword vector
//
// Returns the decoded message.
static inline uint64_t decode_codeword(Matrix *matrix_p, uint64_t vec);
