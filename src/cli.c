#include "cli.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli_internal.h"
#include "matrix.h"
#include "memory.h"

Cli cli_init(int argc, char **argv) {
    Cli cli = {0};
    char *matrix_str = NULL;

    for (short i = 1; i < argc - 1; i++) {
        char *current = argv[i];
        char *next = argv[i + 1];

        try_parse(&cli, current, next, "-p", parse_error_probability);
        try_parse(&cli, current, next, "-n", parse_code_length);
        try_parse(&cli, current, next, "-k", parse_code_dimension);
        try_parse_matrix_str(&matrix_str, current, next);

        try_parse(&cli, current, next, "-s", parse_scenario);
        try_parse(&cli, current, next, "--seed", parse_seed);
    }

    try_parse_matrix(&cli, matrix_str);

    return cli;
}

bool cli_is_valid(Cli *cli_p) {
    Cli cli = *cli_p;

    bool required_arguments_given = cli.code_dimension_given && cli.code_length_given &&
                                    cli.error_probability_given &&
                                    cli.generator_matrix_given;

    if (!required_arguments_given)
        return false;

    bool scenario_in_bounds =
        !cli.scenario_given || (cli.scenario >= 1 && cli.scenario <= 3);

    bool error_probability_in_bounds =
        cli.error_probability <= 1 && cli.error_probability >= 0;

    bool arguments_are_valid =
        scenario_in_bounds && cli.code_dimension <= cli.code_length &&
        error_probability_in_bounds && matrix_is_valid(&cli.generator_matrix);

    if (!arguments_are_valid)
        return false;

    return true;
}

void cli_print_error(Cli *cli_p) {
    assert(cli_p != NULL);

    Cli cli = *cli_p;

#define ERROR_CHECK_AND_PRINT(pred, msg) \
  if ((pred)) {                          \
    puts((msg));                         \
    cli_free(cli_p);                     \
    return;                              \
  }

    ERROR_CHECK_AND_PRINT(
        !cli.error_probability_given,
        "ERROR: error probability (-p) was not provided."
    );

    ERROR_CHECK_AND_PRINT(
        !cli.code_length_given,
        "ERROR: code length (-n) was not provided."
    );

    ERROR_CHECK_AND_PRINT(
        !cli.code_dimension_given,
        "ERROR: code dimension (-k) was not provided."
    );

    ERROR_CHECK_AND_PRINT(
        !cli.generator_matrix_given,
        "ERROR: generator matrix (-g) was not provided."
    );

    ERROR_CHECK_AND_PRINT(
        cli.error_probability < 0 || cli.error_probability > 1,
        "ERROR: error probability must be between 0 and 1."
    );

    ERROR_CHECK_AND_PRINT(
        cli.code_dimension > cli.code_length,
        "ERROR: code dimension cannot be greater than the code length."
    );

    ERROR_CHECK_AND_PRINT(
        !matrix_is_valid(&cli.generator_matrix),
        "ERROR: generator matrix data is invalid. "
        "Possibly caused by incorrect number of elements, or "
        "use of illegal characters."
    );

    ERROR_CHECK_AND_PRINT(
        cli.scenario_given && !(cli.scenario >= 1 && cli.scenario <= 3),
        "ERROR: scenario must be one of these values: 1, 2, 3."
    );

#undef ERROR_CHECK_AND_PRINT
}

void cli_free(Cli *cli_p) {
    safe_free(cli_p->generator_matrix.data);
    safe_free(cli_p->generator_matrix.order);
}

static inline void try_parse(
    Cli *cli_p,
    char *current,
    char *next,
    char *flag,
    Parser parser
) {
    if (strcmp(current, flag) != 0)
        return;

    parser(cli_p, next);
}

static inline void try_parse_matrix_str(char **dest, char *current, char *next) {
    if (strcmp(current, "-g") != 0)
        return;

    *dest = next;
}

static inline void try_parse_matrix(Cli *cli_p, char *str) {
    if (str == NULL)
        return;

    if (!(cli_p->code_dimension_given && cli_p->code_length_given))
        return;

    cli_p->generator_matrix =
        matrix_from_str(str, cli_p->code_dimension, cli_p->code_length);

    cli_p->generator_matrix_given = true;
}

static inline void parse_error_probability(Cli *cli_p, char *str) {
    char *temp_str = safe_strdup(str);

    char *p = strchr(temp_str, ',');
    if (p != NULL)
        *p = '.';

    cli_p->error_probability = atof(temp_str);
    cli_p->error_probability_given = true;

    safe_free(temp_str);
}

static inline void parse_code_length(Cli *cli_p, char *str) {
    cli_p->code_length = atoi(str);
    cli_p->code_length_given = true;
}

static inline void parse_code_dimension(Cli *cli_p, char *str) {
    cli_p->code_dimension = atoi(str);
    cli_p->code_dimension_given = true;
}

static inline void parse_scenario(Cli *cli_p, char *str) {
    cli_p->scenario = atoi(str);
    cli_p->scenario_given = true;
}

static inline void parse_seed(Cli *cli_p, char *str) {
    cli_p->seed = atoi(str);
    cli_p->seed_given = true;
}
