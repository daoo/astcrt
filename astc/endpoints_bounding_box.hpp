#ifndef ENDPOINTS_BOUNDING_BOX_HPP_JI1ZQLPD
#define ENDPOINTS_BOUNDING_BOX_HPP_JI1ZQLPD

#include <stddef.h>

#include "astc/colors.hpp"
#include "astc/constants.hpp"
#include "astc/misc.hpp"
#include "astc/vector.hpp"

void bounding_box(const unorm8_t* texels,
                  size_t count,
                  vec3i_t& e0,
                  vec3i_t& e1) {
  vec3i_t a(255, 255, 255);
  vec3i_t b(0, 0, 0);
  for (size_t i = 0; i < count; ++i) {
    vec3i_t t = to_vec3i(texels[i]);
    a = min(a, t);
    b = max(b, t);
  }
  e0 = a;
  e1 = b;
}

void bounding_box_block(const unorm8_t texels[BLOCK_TEXEL_COUNT],
                        vec3i_t& e0,
                        vec3i_t& e1) {
  bounding_box(texels, BLOCK_TEXEL_COUNT, e0, e1);
}

#endif /* end of include guard: ENDPOINTS_BOUNDING_BOX_HPP_JI1ZQLPD */
