#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cli.h"
#include "scenario1.h"
#include "scenario2.h"
#include "scenario3.h"
#include "state.h"

int main(int argc, char **argv) {
    Cli cli = cli_init(argc, argv);

    if (argc == 1 || cli.help) {
        cli_print_usage(stdout, argv[0]);
        cli_free(&cli);
        return 0;
    }

    if (!cli_is_valid(&cli)) {
        cli_print_usage(stderr, argv[0]);
        cli_free(&cli);
        return 1;
    }

    State state = state_init(&cli);

    size_t seed = cli.seed_given ? cli.seed : time(NULL);
    srand(seed);
    printf("Seed: %zu\n\n", seed);

    switch (cli.scenario) {
    case 0:
        scenario1(&state);
        puts("\nPress Enter to continue...");
        getchar();
        scenario2(&state);
        puts("\nPress Enter to continue...");
        getchar();
        scenario3(&state);
        break;
    case 1:
        scenario1(&state);
        break;
    case 2:
        scenario2(&state);
        break;
    case 3:
        scenario3(&state);
        break;
    }

    cli_free(&cli);
    state_free(&state);
}
