#pragma once

#include "matrix.h"
#include "state.h"
#include <stdint.h>

// Fills state->weights with the coset leader weight of every syndrome.
//
// state - state with parity_check_matrix and coset_count set, and weights
//         allocated with room for coset_count entries
static inline void compute_weights(State *state);
