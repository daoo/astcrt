#ifndef ASTC_PARTITION_HEURISTIC_H_
#define ASTC_PARTITION_HEURISTIC_H_

#include <cstddef>

#include "astc/colors.h"
#include "astc/constants.h"
#include "astc/vector.h"

bool partition_worthit(const unorm8_t texels[BLOCK_TEXEL_COUNT],
                       vec3f_t k,
                       vec3f_t m) {
  float kk = dot(k, k);

  size_t count = 0;
  for (size_t i = 0; i < BLOCK_TEXEL_COUNT; ++i) {
    float d = qd_to_line(m, k, kk, to_vec3f(texels[i]));
    count = count + static_cast<size_t>(d > 100.0f);

    if (count > 2) {
      return true;
    }
  }

  return false;
}

#endif  // ASTC_PARTITION_HEURISTIC_H_
