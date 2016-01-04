#ifndef ASTC_PARTITION_BEST_H_
#define ASTC_PARTITION_BEST_H_

#include <cstddef>
#include <cstdint>

#include "astc/colors.h"
#include "astc/constants.h"
#include "astc/dcheck.h"
#include "astc/partition.h"
#include "astc/partition_heuristic.h"
#include "astc/partition_kmeans.h"

void find_best_partitioning(const unorm8_t texels[BLOCK_TEXEL_COUNT],
                            size_t& partition_index,
                            size_t& partition_count,
                            uint16_t& partition_mask) {
  uint16_t estimated = 0;
  kmeans_clustering_block(texels, estimated);

  int16_t index = partition_2_4x4_lookup_table[estimated];

  if (index == -1) {
    partition_index = 0;
    partition_count = 1;
    partition_mask = partition_1_4x4_mask;
  } else {
    DCHECK(index >= 0 && index < 1024);
    uint16_t mask = partition_2_4x4_mask_table[index];

    // If mask is all zero or one only one partition is needed.
    if (mask == 0 || mask == 0xFFFF) {
      partition_index = 0;
      partition_count = 1;
      partition_mask = partition_1_4x4_mask;
    } else {
      partition_index = static_cast<size_t>(index);
      partition_count = 2;
      partition_mask = mask;
    }
  }
}

#endif  // ASTC_PARTITION_BEST_H_
