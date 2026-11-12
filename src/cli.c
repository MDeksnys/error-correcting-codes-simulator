#include "cli.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli_internal.h"
#include "matrix.h"
#include "memory.h"

const char usage[] =
    "Usage: %s -p <probability> -n <length> -k <dimension> -g <matrix>\n"
    "           [-s <scenario>] [--seed <number>]\n"
    "\n"
    "Required arguments:\n"
    "  -p <probability>  Probability that a bit is flipped, from 0 to 1.\n"
    "  -n <length>       Code length, at most 64.\n"
    "  -k <dimension>    Code dimension, from 1 to n.\n"
    "  -g <matrix>       Generator matrix with k rows and n columns of 0 and 1.\n"
    "                    Rows are separated by commas.\n"
    "\n"
    "Optional arguments:\n"
    "  -s <scenario>     1 = single vector, 2 = text, 3 = BMP image.\n"
    "                    All three run in order if omitted.\n"
    "  --seed <number>   Seed for the random number generator.\n"
    "                    The current time is used if omitted.\n"
    "  -h, --help        Show this message.\n"
    "\n"
    "Example:\n"
    "  %s -p 0.1 -n 7 -k 4 -g 1000101,0100110,0010111,0001011 -s 1\n";

Cli cli_init(int argc, char **argv) {
    Cli cli = {0};
    char *matrix_str = NULL;

    for (int i = 1; i < argc; i++)
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
            cli.help = true;

    for (int i = 1; i < argc - 1; i++) {
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
    bool required_arguments_given = cli_p->code_dimension_given &&
                                    cli_p->code_length_given &&
                                    cli_p->error_probability_given &&
                                    cli_p->generator_matrix_given;

    if (!required_arguments_given)
        return false;

    bool scenario_in_bounds = !cli_p->scenario_given ||
                              (cli_p->scenario >= 1 && cli_p->scenario <= 3);

    bool error_probability_in_bounds = cli_p->error_probability >= 0 &&
                                       cli_p->error_probability <= 1;

    bool dimensions_in_bounds = cli_p->code_dimension >= 1 &&
                                cli_p->code_dimension <= cli_p->code_length &&
                                cli_p->code_length <= 64;

    return scenario_in_bounds &&
           dimensions_in_bounds &&
           error_probability_in_bounds &&
           matrix_is_valid(&cli_p->generator_matrix);
}

void cli_print_usage(FILE *out, const char *program) {
    fprintf(out, usage, program, program);
}

void cli_free(Cli *cli_p) {
    safe_free(cli_p->generator_matrix.rows);
    safe_free(cli_p->generator_matrix.cols);
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
