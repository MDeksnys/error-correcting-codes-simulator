#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "decode.h"
#include "encode.h"
#include "matrix.h"

#ifdef _WIN32
// clang-format off
  #include <windows.h>
  #include <shellapi.h>
// clang-format on
#else
  #include <unistd.h>
#endif

#include "bit_string.h"
#include "channel.h"
#include "cli.h"
#include "memory.h"
#include "scenario3_internal.h"

struct {
  char *filename;
  size_t filesize_bytes;
  size_t pixel_data_length;

  struct {
    size_t length;
    char *data;
  } header;
} metadata;

void
scenario3(Cli *cli_p) {
  assert(cli_is_valid(cli_p));

  puts("--- SCENARIO 3 ---");

  double p_e = cli_p->error_probability;
  uint8_t k = cli_p->code_dimension;
  uint8_t n = cli_p->code_length;
  Matrix *G = &cli_p->generator_matrix;

  calculate_metadata();
  open_file(metadata.filename);

  char *pixel_bytes = extract_pixel_bytes_from_bmp(metadata.filename);
  size_t len = metadata.pixel_data_length;
  const size_t total_bits = len * 8;
  const size_t vec_count = (total_bits / k) + (total_bits % k == 0 ? 0 : 1);
  uint64_t *vecs = safe_malloc(vec_count * sizeof(uint64_t));

  vectors_from_string(vecs, pixel_bytes, len, vec_count, k);

  puts("Sending over channel...");
  uint64_t *received_unencoded_vecs = channel_send_vectors(vecs, vec_count, p_e, k);
  puts("Converting...\n");
  char *received_unencoded_msg =
    bytes_from_vectors(received_unencoded_vecs, vec_count, k, len);

  puts("Encoding...");
  uint64_t *encoded_vecs = encode_all(G, vecs, vec_count);
  puts("Sending over channel...");
  uint64_t *received_encoded_vecs = channel_send_vectors(encoded_vecs, vec_count, p_e, n);
  puts("Decoding...");
  uint64_t *received_decoded_vecs = decode_all(G, received_encoded_vecs, vec_count);
  puts("Converting...");
  char *received_decoded_msg =
    bytes_from_vectors(received_decoded_vecs, vec_count, k, len);

  char filename_unencoded[FILENAME_MAX];
  snprintf(filename_unencoded, FILENAME_MAX, "unencoded_%s", metadata.filename);
  char filename_decoded[FILENAME_MAX];
  snprintf(filename_decoded, FILENAME_MAX, "decoded_%s", metadata.filename);

  write_file(filename_unencoded, received_unencoded_msg);
  write_file(filename_decoded, received_decoded_msg);

  puts("Displaying...");
  open_file(filename_unencoded);
  open_file(filename_decoded);

  safe_free(received_unencoded_vecs);
  safe_free(received_unencoded_msg);
  safe_free(received_decoded_vecs);
  safe_free(received_decoded_msg);
  safe_free(received_encoded_vecs);
  safe_free(encoded_vecs);
  safe_free(metadata.header.data);
  safe_free(vecs);
  safe_free(pixel_bytes);
};

static inline char *
request_bmp_filename(void) {
  char *buffer = safe_malloc(FILENAME_MAX);

  puts("Enter the name of the image (BMP) file:");

  while (true) {
    if (fgets(buffer, FILENAME_MAX, stdin) == NULL)
      continue;

    size_t len = strlen(buffer);
    while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
      buffer[--len] = '\0';

    if (len < 4 || strcmp(buffer + len - 4, ".bmp") != 0) {
      puts("File must have the '.bmp' extension.");
      continue;
    }

    FILE *temp_file;
    if ((temp_file = fopen(buffer, "rb")) == NULL) {
      puts("File doesn't exist.");
      continue;
    }

    fclose(temp_file);
    break;
  }

  return buffer;
}

static inline void
calculate_metadata() {
  metadata.filename = request_bmp_filename();

  FILE *file = fopen(metadata.filename, "rb");
  assert(file != NULL);

  uint8_t *partial_header = safe_malloc(14);
  fread(partial_header, 1, 14, file);
  uint32_t offset_bytes = read_32_at_offset(partial_header, 10);
  safe_free(partial_header);

  rewind(file);
  metadata.header.length = offset_bytes;
  metadata.header.data = safe_malloc(offset_bytes + 1);
  fread(metadata.header.data, 1, offset_bytes, file);

  rewind(file);
  fseek(file, 0, SEEK_END);
  metadata.filesize_bytes = ftell(file);

  metadata.header.data[offset_bytes] = '\0';

  metadata.pixel_data_length = metadata.filesize_bytes - metadata.header.length;

  fclose(file);
}

static inline char *
extract_pixel_bytes_from_bmp(char *filename) {
  FILE *file = fopen(filename, "rb");
  assert(file != NULL);

  char *buffer = safe_malloc(metadata.pixel_data_length);

  // header offset
  fseek(file, metadata.header.length, SEEK_SET);

  fread(buffer, 1, metadata.pixel_data_length, file);
  fclose(file);

  return buffer;
}

static inline void
open_file(char *filename) {
#ifdef _WIN32
  ShellExecuteA(NULL, "open", filename, NULL, NULL, SW_SHOWNORMAL);
#else
  if (fork() == 0)
    execlp("xdg-open", "xdg-open", filename, NULL);
#endif
}

static inline void
write_file(char *filename, char *data) {
  FILE *file = fopen(filename, "wb");
  if (!file)
    return;

  fwrite(metadata.header.data, 1, metadata.header.length, file);
  fwrite(data, 1, metadata.pixel_data_length, file);

  fclose(file);
}

static inline uint32_t
read_32_at_offset(uint8_t *header, uint8_t offset) {
  return ((uint32_t)header[offset]) | ((uint32_t)header[offset + 1] << 8) |
         ((uint32_t)header[offset + 2] << 16) | ((uint32_t)header[offset + 3] << 24);
}
