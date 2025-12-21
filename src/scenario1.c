#include "scenario1.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "channel.h"
#include "cli.h"
#include "decode.h"
#include "encode.h"
#include "memory.h"
#include "scenario1_internal.h"
#include "vector.h"

void
scenario1(Cli *cli_p) {
  assert(cli_is_valid(cli_p));

  puts("--- SCENARIO 1 ---");

  double p_e = cli_p->error_probability;
  uint8_t k = cli_p->code_dimension;
  uint8_t n = cli_p->code_length;
  Matrix *G = &cli_p->generator_matrix;

  char *input = request_input_vector(k);
  char *encoded = encode(G, input);
  char *error = channel_error_vector(p_e, n);
  char *received = vector_add(encoded, error, n);

  print_all_vectors(input, encoded, error, received);

  printf("\nDo you want to use a custom error vector? (y/N) ");

  char input_char = getchar();
  if (input_char == 'y' || input_char == 'Y') {
    safe_free(error);
    safe_free(received);

    error = request_input_vector(n);
    received = vector_add(encoded, error, n);

    print_all_vectors(input, encoded, error, received);
  }

  char *decoded = decode(G, received);

  printf("\nInitial vector: %s\nDecoded vector: %s\n", input, decoded);

  safe_free(input);
  safe_free(encoded);
  safe_free(error);
  safe_free(received);
  safe_free(decoded);
}

static inline char *
request_input_vector(uint8_t length) {
  char *buffer = safe_malloc(length + 1);
  char format[10];
  sprintf(format, "%%%ds", length);

  do {
    printf("Enter a binary vector of length %d: ", length);
    scanf(format, buffer);
    while ((getchar()) != '\n')
      ;
  } while (!vector_is_valid(buffer, length));

  return buffer;
}

static inline void
print_all_vectors(char *input, char *encoded, char *error, char *received) {
  printf(
    "\n"
    "m = %s\n"
    "c = %s = mG\n"
    "e = %s (# of errors: %hhu)\n"
    "y = %s = c + e\n",
    input,
    encoded,
    error,
    vector_weight(error),
    received);
}
