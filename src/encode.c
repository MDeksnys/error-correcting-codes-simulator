#include "encode.h"

#include <stdint.h>
#include <string.h>

#include "matrix.h"
#include "memory.h"

uint64_t encode(Matrix *matrix_p, uint64_t vec) {
    return vector_matrix_mul(vec, matrix_p);
}

uint64_t *encode_all(Matrix *matrix_p, uint64_t *vectors, size_t vec_count) {
    uint64_t *result = safe_malloc(vec_count * sizeof(uint64_t));

    for (size_t i = 0; i < vec_count; i++)
        result[i] = encode(matrix_p, vectors[i]);

    return result;
}
