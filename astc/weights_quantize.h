#ifndef ASTC_WEIGHTS_QUANTIZE_H_
#define ASTC_WEIGHTS_QUANTIZE_H_

#include <cstddef>
#include <cstdint>

#include "astc/colors.h"
#include "astc/constants.h"
#include "astc/dcheck.h"
#include "astc/misc.h"
#include "astc/range.h"
#include "astc/vector.h"
#include "astc/weights_quantize_table.h"

uint8_t quantize_weight(range_t weight_quant, size_t weight) {
  DCHECK(weight_quant <= RANGE_32);
  DCHECK(weight <= 1024);
  return weight_quantize_table[weight_quant][weight];
}

/**
 * Project a texel to a line and quantize the result in 1 dimension.
 *
 * The line is defined by t=k*x + m. This function calculates and quantizes x
 * by projecting n=t-m onto k, x=|n|/|k|. Since k and m is derived from the
 * minimum and maximum of all texel values the result will be in the range [0,
 * 1].
 *
 * To quantize the result using the weight_quantize_table the value needs to
 * be extended to the range [0, 1024].
 *
 * @param k the derivative of the line
 * @param m the minimum endpoint
 * @param t the texel value
 */
size_t project(size_t k, size_t m, size_t t) {
  DCHECK(k > 0);
  return size_t((t - m) * 1024) / k;
}

/**
 * Project a texel to a line and quantize the result in 3 dimensions.
 */
size_t project(vec3i_t k, int kk, vec3i_t m, vec3i_t t) {
  DCHECK(kk > 0);

  return static_cast<size_t>(clamp(0, 1024, dot(t - m, k) * 1024 / kk));
}

void calculate_quantized_weights_luminance(
    const uint8_t texels[BLOCK_TEXEL_COUNT],
    range_t quant,
    uint8_t l0,
    uint8_t l1,
    uint8_t weights[BLOCK_TEXEL_COUNT]) {
  DCHECK(l0 < l1);

  size_t k = l1 - l0;
  size_t m = l0;

  for (size_t i = 0; i < BLOCK_TEXEL_COUNT; ++i) {
    size_t t = static_cast<size_t>(texels[i]);
    weights[i] = quantize_weight(quant, project(k, m, t));
  }
}

void calculate_quantized_weights_rgb_single_partition(
    const unorm8_t texels[BLOCK_TEXEL_COUNT],
    range_t quant,
    const vec3i_t endpoints[2],
    uint8_t weights[BLOCK_TEXEL_COUNT]) {
  if (endpoints[0] == endpoints[1]) {
    for (size_t i = 0; i < BLOCK_TEXEL_COUNT; ++i) {
      weights[i] = 0;  // quantize_weight(quant, 0) is always 0
    }
  } else {
    vec3i_t k = endpoints[1] - endpoints[0];
    vec3i_t m = endpoints[0];

    int kk = dot(k, k);
    for (size_t i = 0; i < BLOCK_TEXEL_COUNT; ++i) {
      weights[i] =
          quantize_weight(quant, project(k, kk, m, to_vec3i(texels[i])));
    }
  }
}

void calculate_quantized_weights_rgb_two_partitions(
    const unorm8_t texels[BLOCK_TEXEL_COUNT],
    const uint16_t partition_mask,
    const vec3i_t endpoints[2][2],
    range_t quant,
    uint8_t weights[BLOCK_TEXEL_COUNT]) {
  vec3i_t ks[2] = {endpoints[0][1] - endpoints[0][0],
                   endpoints[1][1] - endpoints[1][0]};

  int kks[2] = {dot(ks[0], ks[0]), dot(ks[1], ks[1])};

  vec3i_t ms[2] = {endpoints[0][0], endpoints[1][0]};

  for (size_t i = 0; i < BLOCK_TEXEL_COUNT; ++i) {
    size_t j = getbit(partition_mask, i);

    vec3i_t e0 = endpoints[j][0];
    vec3i_t e1 = endpoints[j][1];

    if (e0 == e1) {
      weights[i] = 0;  // quantize_weight(quant, 0) is always 0
    } else {
      vec3i_t k = ks[j];
      vec3i_t m = ms[j];

      int kk = kks[j];
      weights[i] =
          quantize_weight(quant, project(k, kk, m, to_vec3i(texels[i])));
    }
  }
}

#endif  // ASTC_WEIGHTS_QUANTIZE_H_
