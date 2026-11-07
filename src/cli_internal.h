#pragma once

#include "cli.h"

typedef void (*Parser)(Cli *, char *);

// Attempts to parse a command-line argument if the current argument matches a flag.
//
// cli_p   - pointer to the Cli struct to store parsed values
// current - current argument string (flag)
// next    - next argument string (value)
// flag    - expected flag string (e.g., "-p", "-n")
// parser  - function pointer to the actual parser, for the specific flag
//
// If `current` matches `flag`, the `parser` function is invoked with `cli_p` and `next`.
static inline void try_parse(
    Cli *cli_p,
    char *current,
    char *next,
    char *flag,
    Parser parser
);

// Attempts to capture the generator matrix string from command-line arguments.
//
// dest    - pointer to a char * where the matrix string will be stored
// current - current argument string (flag)
// next    - next argument string (matrix data)
//
// If `current` matches "-g", `next` is stored in `*dest`.
// Otherwise, the function does nothing.
static inline void try_parse_matrix_str(char **dest, char *current, char *next);

// Attempts to parse the generator matrix from a string.
//
// cli_p - pointer to the Cli struct to store the parsed matrix
// str   - string representing the data of the generator matrix
//
// The function only parses the matrix if `str` is not NULL and both code dimension and
// code length have been provided.
static inline void try_parse_matrix(Cli *cli_p, char *str);

// Parses and sets the error probability from a string.
//
// cli_p - pointer to the Cli struct to store the parsed value
// str   - string representing the error probability
static inline void parse_error_probability(Cli *cli_p, char *str);

// Parses and sets the code length from a string.
//
// cli_p - pointer to the Cli struct to store the parsed value
// str   - string representing the code length
static inline void parse_code_length(Cli *cli_p, char *str);

// Parses and sets the code dimension from a string.
//
// cli_p - pointer to the Cli struct to store the parsed value
// str   - string representing the code dimension
static inline void parse_code_dimension(Cli *cli_p, char *str);

// Parses and sets the scenario number from a string.
//
// cli_p - pointer to the Cli struct to store the parsed value
// str   - string representing the scenario number
static inline void parse_scenario(Cli *cli_p, char *str);

// Parses and sets the random seed from a string.
//
// cli_p - pointer to the Cli struct to store the parsed value
// str   - string representing the seed
static inline void parse_seed(Cli *cli_p, char *str);
