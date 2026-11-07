#pragma once

#include <stdint.h>

// Prompts the user to enter the name of a BMP file and validates it.
//
// Returns a newly allocated string containing the validated filename.
// Caller is responsible for freeing the returned memory.
static inline char *
request_bmp_filename(void);

// Reads and calculates metadata for a BMP file.
//
// Populates the global metadata variable with the following fields:
// - filename: the validated BMP filename (dynamically allocated)
// - filesize_bytes: total size of the BMP file
// - header.data: dynamically allocated copy of the BMP header
// - header.length: length of the BMP header in bytes
// - pixel_data_length: length of the pixel data in bytes
//
// Caller is responsible for freeing metadata.filename and metadata.header.data.
static inline void
calculate_metadata(void);

// Reads the pixel data of a BMP file.
//
// filename - path to the BMP file
//
// Returns a newly allocated buffer of metadata.pixel_data_length bytes.
// Caller is responsible for freeing the returned memory.
static inline char *
extract_pixel_bytes_from_bmp(char *filename);

// Opens a file using the system's default application.
//
// filename - path to the file to open
static inline void
open_file(char *filename);

// Writes a BMP file with the given pixel data while preserving the original header.
// Existing file at filename will be overwritten.
//
// filename - path to the output BMP file
// data     - pointer to the pixel data to write
static inline void
write_file(char *filename, char *data);

// Reads 32 bits from a byte buffer at the specified offset.
//
// header - pointer to the byte buffer
// offset - index in the buffer from which to read the 4 bytes
//
// Returns a 32-bit value.
static inline uint32_t
read_32_at_offset(uint8_t *header, uint8_t offset);
