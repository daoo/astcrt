#ifndef ASTC_ENDPOINTS_BOUNDING_BOX_H_
#define ASTC_ENDPOINTS_BOUNDING_BOX_H_

#include <cstddef>

#include "astc/colors.h"
#include "astc/constants.h"
#include "astc/misc.h"
#include "astc/vector.h"

void bounding_box(const unorm8_t* texels,
                  size_t count,
                  vec3i_t& e0,
                  vec3i_t& e1) {
  vec3i_t a(255, 255, 255);
  vec3i_t b(0, 0, 0);
  for (size_t i = 0; i < count; ++i) {
    vec3i_t t = to_vec3i(texels[i]);
    a = vecmin(a, t);
    b = vecmax(b, t);
  }
  e0 = a;
  e1 = b;
}

void bounding_box_block(const unorm8_t texels[BLOCK_TEXEL_COUNT],
                        vec3i_t& e0,
                        vec3i_t& e1) {
  bounding_box(texels, BLOCK_TEXEL_COUNT, e0, e1);
}

void bounding_box_optimal_diagonal(const unorm8_t* texels,
                                   size_t count,
                                   vec3i_t& e0,
                                   vec3i_t& e1) {
  vec3i_t a, b;
  bounding_box(texels, count, a, b);

  vec3i_t p000(a.x, a.y, a.z);
  vec3i_t p001(a.x, a.y, b.z);
  vec3i_t p010(a.x, b.y, a.z);
  vec3i_t p011(a.x, b.y, b.z);
  vec3i_t p100(b.x, a.y, a.z);
  vec3i_t p101(b.x, a.y, b.z);
  vec3i_t p110(b.x, b.y, a.z);
  vec3i_t p111(b.x, b.y, b.z);

  vec3i_t diag[4][2] = {{p000, p111}, {p010, p101}, {p110, p001}, {p100, p011}};

  vec3i_t k[4] = {
      diag[0][1] - diag[0][0],
      diag[1][1] - diag[1][0],
      diag[2][1] - diag[2][0],
      diag[3][1] - diag[3][0],
  };

  int kk[4] = {
      dot(k[0], k[0]), dot(k[1], k[1]), dot(k[2], k[2]), dot(k[3], k[3])};

  int cost[4] = {0, 0, 0, 0};
  for (size_t i = 0; i < count; ++i) {
    vec3i_t t = to_vec3i(texels[i]);
    cost[0] += qd_to_line(k[0], diag[0][0], kk[0], t);
    cost[1] += qd_to_line(k[1], diag[1][0], kk[1], t);
    cost[2] += qd_to_line(k[2], diag[2][0], kk[2], t);
    cost[3] += qd_to_line(k[3], diag[3][0], kk[3], t);
  }

  int ix = min4_index(cost);

  e0 = diag[ix][0];
  e1 = diag[ix][1];
}

void bounding_box_optimal_diagonal_block(
    const unorm8_t texels[BLOCK_TEXEL_COUNT],
    vec3i_t& e0,
    vec3i_t& e1) {
  bounding_box_optimal_diagonal(texels, BLOCK_TEXEL_COUNT, e0, e1);
}

#endif  // ASTC_ENDPOINTS_BOUNDING_BOX_H_
