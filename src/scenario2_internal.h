#pragma once

// Reads multi-line input from the user until a sentinel line is entered.
//
// Returns a newly allocated null-terminated string containing the full input.
// Caller is responsible for freeing the returned memory.
static inline char *request_input(void);
