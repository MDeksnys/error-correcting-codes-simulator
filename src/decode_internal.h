#pragma once

#include "state.h"
#include <stdint.h>
#include <stddef.h>

// Corrects errors in a received vector using step-by-step decoding algorithm.
//
// state_p - pointer to the state
// vec      - binary vector to correct
//
// Returns the corrected vector, a valid codeword for the code.
static inline uint64_t step_by_step(State *state_p, uint64_t vec);

// Decodes a corrected codeword vector into its original message.
//
// state_p - pointer to the state
// vec      - corrected codeword vector
//
// Returns the decoded message.
static inline uint64_t decode_codeword(State *state_p, uint64_t vec);
