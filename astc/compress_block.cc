#include "astc/compress_block.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>

#include "astc/colors.h"
#include "astc/constants.h"
#include "astc/data_size.h"
#include "astc/endpoints.h"
#include "astc/endpoints_encode.h"
#include "astc/endpoints_min_max.h"
#include "astc/endpoints_principal_components.h"
#include "astc/integer_sequence_encoding.h"
#include "astc/misc.h"
#include "astc/partition.h"
#include "astc/partition_best.h"
#include "astc/partition_heuristic.h"
#include "astc/range.h"
#include "astc/store_block.h"
#include "astc/vector.h"
#include "astc/weights_quantize.h"

/**
 * Write void extent block bits for LDR mode and unused extent coordinates.
 */
void encode_void_extent(vec3i_t color, PhysicalBlock* physical_block) {
  void_extent_to_physical(unorm8_to_unorm16(to_unorm8(color)), physical_block);
}

void encode_luminance(const uint8_t texels[BLOCK_TEXEL_COUNT],
                      PhysicalBlock* physical_block) {
  size_t partition_count = 1;
  size_t partition_index = 0;

  color_endpoint_mode_t color_endpoint_mode = CEM_LDR_LUMINANCE_DIRECT;
  range_t weight_quant = RANGE_32;
  range_t endpoint_quant =
      endpoint_quantization(partition_count, weight_quant, color_endpoint_mode);

  uint8_t l0 = 255;
  uint8_t l1 = 0;
  for (size_t i = 0; i < BLOCK_TEXEL_COUNT; ++i) {
    l0 = std::min(l0, texels[i]);
    l1 = std::max(l1, texels[i]);
  }

  uint8_t endpoint_unquantized[2];
  uint8_t endpoint_quantized[2];
  encode_luminance_direct(endpoint_quant, l0, l1, endpoint_quantized,
                          endpoint_unquantized);

  uint8_t weights_quantized[BLOCK_TEXEL_COUNT];
  calculate_quantized_weights_luminance(
      texels, weight_quant, endpoint_unquantized[0], endpoint_unquantized[1],
      weights_quantized);

  uint8_t endpoint_ise[MAXIMUM_ENCODED_COLOR_ENDPOINT_BYTES] = {0};
  integer_sequence_encode(endpoint_quantized, 2, RANGE_256, endpoint_ise);

  uint8_t weights_ise[MAXIMUM_ENCODED_WEIGHT_BYTES + 1] = {0};
  integer_sequence_encode(weights_quantized, BLOCK_TEXEL_COUNT, RANGE_32,
                          weights_ise);

  symbolic_to_physical(color_endpoint_mode, endpoint_quant, weight_quant,
                       partition_count, partition_index, endpoint_ise,
                       weights_ise, physical_block);
}

void encode_rgb_single_partition(const unorm8_t texels[BLOCK_TEXEL_COUNT],
                                 vec3f_t endpoints[2],
                                 PhysicalBlock* physical_block) {
  size_t partition_index = 0;
  size_t partition_count = 1;

  color_endpoint_mode_t color_endpoint_mode = CEM_LDR_RGB_DIRECT;
  range_t weight_quant = RANGE_12;
  range_t endpoint_quant =
      endpoint_quantization(partition_count, weight_quant, color_endpoint_mode);

  vec3i_t endpoint_unquantized[2];
  uint8_t endpoint_quantized[6];
  encode_rgb_direct(endpoint_quant, round(endpoints[0]), round(endpoints[1]),
                    endpoint_quantized, endpoint_unquantized);

  uint8_t weights_quantized[BLOCK_TEXEL_COUNT];
  calculate_quantized_weights_rgb_single_partition(
      texels, weight_quant, endpoint_unquantized, weights_quantized);

  uint8_t endpoint_ise[MAXIMUM_ENCODED_COLOR_ENDPOINT_BYTES] = {0};
  integer_sequence_encode(endpoint_quantized, 6, endpoint_quant, endpoint_ise);

  uint8_t weights_ise[MAXIMUM_ENCODED_WEIGHT_BYTES + 1] = {0};
  integer_sequence_encode(weights_quantized, BLOCK_TEXEL_COUNT, weight_quant,
                          weights_ise);

  symbolic_to_physical(color_endpoint_mode, endpoint_quant, weight_quant,
                       partition_count, partition_index, endpoint_ise,
                       weights_ise, physical_block);
}

void encode_rgb_two_partitions(const unorm8_t texels[BLOCK_TEXEL_COUNT],
                               size_t partition_index,
                               uint16_t partition_mask,
                               vec3f_t endpoints[2][2],
                               PhysicalBlock* physical_block) {
  size_t partition_count = 2;

  color_endpoint_mode_t color_endpoint_mode = CEM_LDR_RGB_DIRECT;
  range_t weight_quant = RANGE_8;
  range_t endpoint_quant =
      endpoint_quantization(partition_count, weight_quant, color_endpoint_mode);

  vec3i_t endpoint_unquantized[2][2];
  uint8_t endpoint_quantized[12];
  encode_rgb_direct(endpoint_quant, round(endpoints[0][0]),
                    round(endpoints[0][1]), endpoint_quantized,
                    endpoint_unquantized[0]);
  encode_rgb_direct(endpoint_quant, round(endpoints[1][0]),
                    round(endpoints[1][1]), endpoint_quantized + 6,
                    endpoint_unquantized[1]);

  uint8_t weights_quantized[BLOCK_TEXEL_COUNT];
  calculate_quantized_weights_rgb_two_partitions(
      texels, partition_mask, endpoint_unquantized, weight_quant,
      weights_quantized);

  uint8_t endpoint_ise[MAXIMUM_ENCODED_COLOR_ENDPOINT_BYTES] = {0};
  integer_sequence_encode(endpoint_quantized, 12, endpoint_quant, endpoint_ise);

  uint8_t weights_ise[MAXIMUM_ENCODED_WEIGHT_BYTES + 1] = {0};
  integer_sequence_encode(weights_quantized, BLOCK_TEXEL_COUNT, weight_quant,
                          weights_ise);

  symbolic_to_physical(color_endpoint_mode, endpoint_quant, weight_quant,
                       partition_count, partition_index, endpoint_ise,
                       weights_ise, physical_block);
}

bool is_solid(const unorm8_t texels[BLOCK_TEXEL_COUNT],
              size_t count,
              unorm8_t* color) {
  for (size_t i = 0; i < count; ++i) {
    if (!approx_equal(to_vec3i(texels[i]), to_vec3i(texels[0]))) {
      return false;
    }
  }

  // TODO: Calculate average color?
  *color = texels[0];
  return true;
}

bool is_greyscale(const unorm8_t texels[BLOCK_TEXEL_COUNT],
                  size_t count,
                  uint8_t luminances[BLOCK_TEXEL_COUNT]) {
  for (size_t i = 0; i < count; ++i) {
    vec3i_t color = to_vec3i(texels[i]);
    luminances[i] = static_cast<uint8_t>(luminance(color));
    vec3i_t lum(luminances[i], luminances[i], luminances[i]);
    if (!approx_equal(color, lum)) {
      return false;
    }
  }

  return true;
}

void compute_endpoints(const partitioning& part, vec3f_t endpoints[2]) {
  if (part.count == 2) {
    endpoints[0] = to_vec3f(part.texels[0]);
    endpoints[1] = to_vec3f(part.texels[1]);
    return;
  }

  unorm8_t color;
  if (is_solid(part.texels, part.count, &color)) {
    endpoints[0] = to_vec3f(color);
    endpoints[1] = to_vec3f(color);
    return;
  }

  vec3f_t k, m;
  principal_component_analysis(part.texels, part.count, k, m);
  find_min_max(part.texels, part.count, k, m, endpoints[0], endpoints[1]);
}

void compress_block(const unorm8_t texels[BLOCK_TEXEL_COUNT],
                    PhysicalBlock* physical_block) {
  {
    unorm8_t color;
    if (is_solid(texels, BLOCK_TEXEL_COUNT, &color)) {
      encode_void_extent(to_vec3i(color), physical_block);
      /* encode_void_extent(vec3i_t(0, 0, 0), physical_block); */
      return;
    }
  }

  {
    uint8_t luminances[BLOCK_TEXEL_COUNT];
    if (is_greyscale(texels, BLOCK_TEXEL_COUNT, luminances)) {
      encode_luminance(luminances, physical_block);
      /* encode_void_extent(vec3i_t(255, 0, 0), physical_block); */
      return;
    }
  }

  vec3f_t k, m;
  principal_component_analysis_block(texels, k, m);
  if (partition_worthit(texels, k, m)) {
    size_t partition_index = 0;
    size_t partition_count = 0;
    uint16_t partition_mask = 0;

    find_best_partitioning(texels, partition_index, partition_count,
                           partition_mask);

    if (partition_count == 2) {
      partitioning parts[2];
      partition_2(texels, partition_mask, parts);

      vec3f_t endpoints[2][2];

      compute_endpoints(parts[0], endpoints[0]);
      compute_endpoints(parts[1], endpoints[1]);

      encode_rgb_two_partitions(texels, partition_index, partition_mask,
                                endpoints, physical_block);
      /* encode_void_extent(vec3i_t(0, 0, 255), physical_block); */
      return;
    }

    DCHECK(partition_count == 1);
    /* encode_void_extent(vec3i_t(255, 0, 255), physical_block); */
    /* return; */
  }

  vec3f_t endpoints[2];
  find_min_max_block(texels, k, m, endpoints[0], endpoints[1]);
  encode_rgb_single_partition(texels, endpoints, physical_block);
  /* encode_void_extent(vec3i_t(0, 255, 0), physical_block); */
}
