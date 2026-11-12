#include "decode.h"

#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "decode_internal.h"
#include "matrix.h"
#include "memory.h"
#include "state.h"
#include "vector.h"

uint64_t decode(State *state_p, uint64_t vec) {
    uint64_t codeword = step_by_step(state_p, vec);
    return decode_codeword(state_p, codeword);
}

uint64_t *decode_all(State *state_p, uint64_t *vectors, size_t vec_count) {
    uint64_t *result = safe_malloc(vec_count * sizeof(uint64_t));

    #pragma omp parallel for
    for (size_t i = 0; i < vec_count; i++)
        result[i] = decode(state_p, vectors[i]);

    return result;
}

static inline uint64_t step_by_step(State *state_p, uint64_t vec) {
    uint64_t work_vec = vec;

    uint64_t syndrome = vector_syndrome(&state_p->parity_check_matrix, work_vec);
    uint8_t current_weight = state_p->weights[syndrome];

    uint8_t i = 0;
    while (current_weight != 0) {
        uint64_t temp_vec = 0;
        temp_vec |= (uint64_t)1 << i;

        uint64_t sum = temp_vec ^ work_vec;

        syndrome = vector_syndrome(&state_p->parity_check_matrix, sum);
        uint8_t temp_weight = state_p->weights[syndrome];

        if (temp_weight < current_weight) {
            work_vec = sum;
            current_weight = temp_weight;
        }

        i++;
    }

    return work_vec;
}

static inline uint64_t decode_codeword(State *state_p, uint64_t vec) {
    uint8_t row_count = state_p->generator_matrix.row_count;

    uint64_t work_vec = 0;
    uint8_t *positions = safe_malloc(row_count);

    for (size_t weight = 0; weight <= row_count; weight++) {
        for (size_t i = 0; i < weight; i++)
            positions[i] = i;

        work_vec = vector_from_positions(positions, weight);

        do {
            uint64_t product = vector_matrix_mul(work_vec, &state_p->generator_matrix);

            if (product == vec) {
                safe_free(positions);
                return work_vec;
            }
        } while (next_vector_with_weight(&work_vec, &positions, weight, row_count));
    }

    assert(false && "unreachable: provided vec must be a valid codeword");
    return 0;
}
