#include "scenario1.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "channel.h"
#include "decode.h"
#include "encode.h"
#include "matrix.h"
#include "scenario1_internal.h"
#include "state.h"
#include "vector.h"

void scenario1(State *state_p) {
    puts("--- SCENARIO 1 ---");

    double p_e = state_p->error_probability;
    uint8_t k = state_p->k;
    uint8_t n = state_p->n;
    Matrix *G = &state_p->generator_matrix;

    uint64_t input = request_input_vector(k);
    uint64_t encoded = encode(G, input);
    uint64_t error = channel_error_vector(p_e, n);
    uint64_t received = encoded ^ error;

    print_all_vectors(k, n, input, encoded, error, received);

    printf("\nDo you want to use a custom error vector? (y/N) ");

    int input_char = getchar();
    if (input_char != '\n' && input_char != EOF)
        discard_line();

    if (input_char == 'y' || input_char == 'Y') {
        error = request_input_vector(n);
        received = encoded ^ error;

        print_all_vectors(k, n, input, encoded, error, received);
    }

    uint64_t decoded = decode(state_p, received);

    char input_str[65];
    char decoded_str[65];
    vector_format(input, k, input_str);
    vector_format(decoded, k, decoded_str);

    printf("\nInitial vector: %s\nDecoded vector: %s\n", input_str, decoded_str);
}

static inline void discard_line(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

static inline uint64_t request_input_vector(uint8_t length) {
    char buffer[65];
    ParseResult res;

    do {
        printf("Enter a binary vector of length %d: ", length);
        if (scanf("%64s", buffer) != 1)
            exit(EXIT_FAILURE);

        buffer[length] = '\0';
        discard_line();

        res = vector_parse(buffer, length);
    } while (!res.ok);

    return res.vec;
}

static inline void print_all_vectors(
    uint8_t k,
    uint8_t n,
    uint64_t input,
    uint64_t encoded,
    uint64_t error,
    uint64_t received
) {
    char m[65], c[65], e[65], y[65];
    vector_format(input, k, m);
    vector_format(encoded, n, c);
    vector_format(error, n, e);
    vector_format(received, n, y);

    printf(
        "\n"
        "m = %s\n"
        "c = %s = mG\n"
        "e = %s (# of errors: %hhu)\n"
        "y = %s = c + e\n",
        m,
        c,
        e,
        vector_weight(error),
        y
    );
}
