#ifndef CONSTANTS_H_FG9WHVO2
#define CONSTANTS_H_FG9WHVO2

#include <stddef.h>

const size_t BLOCK_WIDTH = 4;
const size_t BLOCK_HEIGHT = 4;
const size_t BLOCK_TEXEL_COUNT = BLOCK_WIDTH * BLOCK_HEIGHT;
const size_t BLOCK_BYTES = 16;

const size_t MAXIMUM_ENCODED_WEIGHT_BITS = 96;
const size_t MAXIMUM_ENCODED_WEIGHT_BYTES = 12;

const size_t MAXIMUM_ENCODED_COLOR_ENDPOINT_BYTES = 12;

const size_t MAX_ENDPOINT_VALUE_COUNT = 18;

#endif /* end of include guard: CONSTANTS_H_FG9WHVO2 */
