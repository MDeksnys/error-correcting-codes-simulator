#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "cli.h"

typedef struct {
    uint8_t n;
    uint8_t k;
    double error_probability;
    Matrix generator_matrix;
    Matrix parity_check_matrix;
    size_t coset_count;
    uint8_t *weights;
} State;

// Builds the state from valid command-line arguments.
//
// cli_p - pointer to a valid Cli
//
// Returns the state.
// Caller is responsible for calling state_free.
State state_init(Cli *cli_p);

// Frees the memory held by an State.
//
// state_p - pointer to an State
void state_free(State *state_p);
