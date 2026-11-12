#include "state.h"

#include "state_internal.h"
#include "matrix.h"
#include "memory.h"
#include "vector.h"

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

State state_init(Cli *cli_p) {
    const uint8_t n = cli_p->code_length;
    const uint8_t k = cli_p->code_dimension;

    State state = {
        .n = n,
        .k = k,
        .error_probability = cli_p->error_probability
    };

    state.generator_matrix = cli_p->generator_matrix;
    cli_p->generator_matrix = (Matrix){0};

    state.parity_check_matrix = matrix_parity_check(&state.generator_matrix);

    state.coset_count = (size_t)1 << (state.n - state.k);
    state.weights = safe_malloc(state.coset_count * sizeof(*state.weights));

    compute_weights(&state);

    return state;
}

void state_free(State *state_p) {
    safe_free(state_p->generator_matrix.data);
    safe_free(state_p->generator_matrix.order);
    safe_free(state_p->parity_check_matrix.data);
    safe_free(state_p->parity_check_matrix.order);
    safe_free(state_p->weights);
}

static inline void compute_weights(State *state) {
    uint8_t *weights = state->weights;
    Matrix parity_check_matrix = state->parity_check_matrix;
    uint8_t col_count = parity_check_matrix.col_count;
    size_t coset_count = state->coset_count;

    memset(weights, UINT8_MAX, coset_count);

    uint8_t *positions = safe_malloc(col_count);
    size_t unique_syndrome_count = 0;

    for (size_t weight = 0; weight <= col_count && unique_syndrome_count != coset_count;
         weight++) {
        for (size_t i = 0; i < weight; i++)
            positions[i] = i;

        uint64_t work_vec = vector_from_positions(positions, weight);

        do {
            uint64_t syndrome = vector_syndrome(&parity_check_matrix, work_vec);

            if (weights[syndrome] == UINT8_MAX) {
                weights[syndrome] = weight;
                unique_syndrome_count++;
            }
        } while (unique_syndrome_count != coset_count &&
                 next_vector_with_weight(&work_vec, &positions, weight, col_count));
    }

    safe_free(positions);
}
