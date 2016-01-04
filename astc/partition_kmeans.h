#ifndef ASTC_PARTITION_KMEANS_H_
#define ASTC_PARTITION_KMEANS_H_

#include <cstddef>
#include <cstdint>

#include "astc/bitmanip.h"
#include "astc/colors.h"
#include "astc/constants.h"
#include "astc/vector.h"

void furthest_apart(const unorm8_t texels[BLOCK_TEXEL_COUNT],
                    vec3i_t& e0,
                    vec3i_t& e1) {
  int d = 0;
  for (size_t i = 0; i < BLOCK_TEXEL_COUNT; ++i) {
    vec3i_t a = to_vec3i(texels[i]);
    for (size_t j = 0; j < BLOCK_TEXEL_COUNT; ++j) {
      vec3i_t b = to_vec3i(texels[j]);
      int dn = qd(a, b);
      if (dn > d) {
        e0 = a;
        e1 = b;
        d = dn;
      }
    }
  }
}

void kmeans_clustering_block(const unorm8_t texels[BLOCK_TEXEL_COUNT],
                             uint16_t& mask) {
  vec3i_t center0, center1;
  furthest_apart(texels, center0, center1);

  for (size_t i = 0; i < 4; ++i) {
    vec3i_t sum0(0, 0, 0);
    vec3i_t sum1(0, 0, 0);
    int count0 = 0;
    int count1 = 0;

    for (size_t j = 0; j < BLOCK_TEXEL_COUNT; ++j) {
      vec3i_t color = to_vec3i(texels[j]);

      int dc0 = qd(color, center0);
      int dc1 = qd(color, center1);

      if (dc0 < dc1) {
        sum0 = sum0 + color;
        count0 = count0 + 1;
        setbit(mask, j, 0);
      } else {
        sum1 = sum1 + color;
        count1 = count1 + 1;
        setbit(mask, j, 1);
      }
    }

    if (count0 > 0) {
      center0 = sum0 / count0;
    }

    if (count1 > 0) {
      center1 = sum1 / count1;
    }
  }
}

#endif  // ASTC_PARTITION_KMEANS_H_
