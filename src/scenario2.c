#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "bit_string.h"
#include "channel.h"
#include "cli.h"
#include "decode.h"
#include "encode.h"
#include "memory.h"

#define MAXIMUM_INPUT_LENGTH 1024

#include "scenario2_internal.h"

void
scenario2(Cli *cli_p) {
  assert(cli_is_valid(cli_p));

  puts("--- SCENARIO 2 ---");

  double p_e = cli_p->error_probability;
  uint8_t k = cli_p->code_dimension;
  uint8_t n = cli_p->code_length;
  Matrix *G = &cli_p->generator_matrix;

  puts(
    "Enter text below. "
    "To finish, type a single dot '.' on its own line and press Enter.");

  char *input = request_input();
  char *bits = bit_string_from_bytes(input, strlen(input));
  const size_t input_length = strlen(bits);

  const size_t vec_count = (input_length / k) + (input_length % k == 0 ? 0 : 1);

  char **bit_vecs = safe_malloc(vec_count * sizeof(char *));

  uint8_t count_appended =
    vectors_from_bit_string(&bit_vecs, bits, input_length, vec_count, k);

  puts("Sending over channel...");
  char **received_unencoded_vecs = channel_send_vectors(bit_vecs, vec_count, p_e, k);
  puts("Converting...");
  char *received_unencoded_bits =
    bit_string_from_vectors(received_unencoded_vecs, count_appended, vec_count, k);
  char *received_unencoded_msg = bytes_from_bit_string(received_unencoded_bits);

  puts("Encoding...");
  char **encoded_vecs = encode_all(G, bit_vecs, vec_count);
  puts("Sending over channel...");
  char **received_encoded_vecs = channel_send_vectors(encoded_vecs, vec_count, p_e, n);
  puts("Decoding...");
  char **received_decoded_vecs = decode_all(G, received_encoded_vecs, vec_count);
  puts("Converting...");
  char *received_decoded_bits =
    bit_string_from_vectors(received_decoded_vecs, count_appended, vec_count, k);
  char *received_decoded_msg = bytes_from_bit_string(received_decoded_bits);

  printf("\nOriginal:\n%s\n", input);
  printf("\nNot encoded:\n%s\n", received_unencoded_msg);
  printf("\nEncoded and decoded:\n%s\n", received_decoded_msg);

  for (size_t i = 0; i < vec_count; i++)
    safe_free(bit_vecs[i]);
  safe_free(bit_vecs);

  safe_free(input);
  safe_free(bits);

  for (size_t i = 0; i < vec_count; i++)
    safe_free(received_unencoded_vecs[i]);
  safe_free(received_unencoded_vecs);
  safe_free(received_unencoded_bits);
  safe_free(received_unencoded_msg);

  for (size_t i = 0; i < vec_count; i++) {
    safe_free(encoded_vecs[i]);
    safe_free(received_encoded_vecs[i]);
    safe_free(received_decoded_vecs[i]);
  }

  safe_free(encoded_vecs);
  safe_free(received_encoded_vecs);
  safe_free(received_decoded_vecs);
  safe_free(received_decoded_bits);
  safe_free(received_decoded_msg);
};

static inline char *
request_input(void) {
  char *buffer = safe_malloc(MAXIMUM_INPUT_LENGTH);
  buffer[0] = '\0';

  const char *sentinel = ".\n";
  size_t used = 0;

  while (true) {
    char line[MAXIMUM_INPUT_LENGTH];

    if (fgets(line, MAXIMUM_INPUT_LENGTH, stdin) == NULL)
      break;

    if (strcmp(line, sentinel) == 0) {
      // Ignore previous newline (cleaner print formatting)
      if (used > 0)
        buffer[used - 1] = '\0';

      break;
    }

    size_t length = strlen(line);
    if (used + length + 1 >= MAXIMUM_INPUT_LENGTH) {
      length = MAXIMUM_INPUT_LENGTH - used - 1;
      memcpy(buffer + used, line, length);
      buffer[MAXIMUM_INPUT_LENGTH] = '\0';

      break;
    }

    memcpy(buffer + used, line, length);
    used += length;
    buffer[used] = '\0';
  }

  return buffer;
}
