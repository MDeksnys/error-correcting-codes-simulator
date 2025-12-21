#pragma once

#include "matrix.h"

// Computes all unique syndromes and their associated weights for a given
// parity-check matrix.
//
// syndromes_p            - pointer to an array of strings to store unique syndromes
// weights_p              - pointer to an array to store weights corresponding to
//                          each syndrome
// parity_check_matrix_p  - pointer to the parity-check matrix
// coset_count            - total number of cosets (the number of unique syndromes)
//
// Populates the provided arrays with unique syndromes and their associated weights.
static inline void
compute_syndromes_and_weights(char ***syndromes_p,
                              uint8_t **weights_p,
                              Matrix *parity_check_matrix_p,
                              size_t coset_count);

// Checks if a syndrome is present in the array of syndromes.
//
// syndromes - array of syndrome strings
// syndrome  - syndrome to search for
// count     - number of syndromes currently in the array
//
// Returns true if the syndrome is in the array, false otherwise.
static inline bool
syndromes_contains(char **syndromes, char *syndrome, size_t count);

// Finds the weight associated with a given syndrome.
//
// syndrome  - syndrome string to search for
// syndromes - array of syndrome strings
// weights   - array of weights corresponding to syndromes
// count     - number of syndromes in the array
//
// Returns the weight of the associated syndrome, or UINT8_MAX if
// not found (unreachable if syndromes and weights are correct).
static inline uint8_t
find_associated_weight(char *syndrome, char **syndromes, uint8_t *weights, size_t count);


// Corrects errors in a received vector using step-by-step decoding algorithm.
//
// matrix_p - pointer to a valid generator matrix
// vec      - binary vector to correct
//
// Returns a newly allocated null-terminated string representing the corrected vector
// a valid codeword for the code generated with matrix_p.
// Caller is responsible for freeing the returned memory.
static inline char *
step_by_step(Matrix *matrix_p, char *vec);

// Decodes a corrected codeword vector into its original message.
//
// matrix_p - pointer to a valid generator matrix
// vec      - corrected codeword vector
//
// Returns a newly allocated null-terminated string containing the decoded message.
// Caller is responsible for freeing the returned memory.
static inline char *
decode_codeword(Matrix *matrix_p, char *vec);
