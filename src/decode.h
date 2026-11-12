#pragma once

#include "state.h"
#include <stdint.h>
#include <stddef.h>

// Decodes a single binary vector.
//
// state_p - pointer to the state
// vec     - the binary vector to decode
//
// Returns the decoded vector.
uint64_t decode(State *state_p, uint64_t vec);

// Decodes an array of binary vectors.
//
// state_p   - pointer to the state
// vectors   - array of binary vectors
// vec_count - number of vectors in the array
//
// Returns a newly allocated array of decoded vectors.
// Caller is responsible for freeing the returned array.
uint64_t *decode_all(State *state_p, uint64_t *vectors, size_t vec_count);
