#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "matrix.h"

typedef struct {
    // required arguments

    double error_probability;
    bool error_probability_given;

    uint8_t code_length;
    bool code_length_given;

    uint8_t code_dimension;
    bool code_dimension_given;

    Matrix generator_matrix;
    bool generator_matrix_given;

    // optional arguments

    uint8_t scenario;
    bool scenario_given;

    uint64_t seed;
    bool seed_given;
} Cli;

// Parses command-line arguments into a Cli struct.
//
// argc, argv - standard command-line argument count and array
//
// Returns a Cli struct with fields set according to the provided arguments.
// Memory allocated for the generator matrix (if provided) must later be freed.
Cli cli_init(int argc, char **argv);

// Checks whether the provided Cli struct is in a valid state.
//
// cli_p - pointer to a Cli struct to validate
//
// Returns true if all required arguments are present and valid:
//   - code dimension, code length, and error probability were provided
//   - generator matrix is provided and valid
//   - scenario (if given) is within [1, 3]
//   - code dimension does not exceed code length
//   - error probability is between 0 and 1
// Returns false if any of these conditions are not met.
bool cli_is_valid(Cli *cli_p);

// Prints an appropriate error message describing why the provided Cli
// struct is invalid.
//
// cli_p - pointer to an invalid Cli struct
void cli_print_error(Cli *cli_p);

// Frees dynamically allocated memory inside a Cli struct.
//
// cli_p - pointer to a Cli struct
void cli_free(Cli *cli_p);
