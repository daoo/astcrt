#ifndef COMPRESS_BLOCK_H_ATUDCKX5
#define COMPRESS_BLOCK_H_ATUDCKX5

#include "astc/constants.hpp"

union unorm8_t;
struct PhysicalBlock;

void compress_block(
    const unorm8_t texels[BLOCK_TEXEL_COUNT],
    PhysicalBlock* physical_block);

#endif /* end of include guard: COMPRESS_BLOCK_H_ATUDCKX5 */
