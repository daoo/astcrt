#ifndef ASTC_ENDPOINTS_ENCODE_H_
#define ASTC_ENDPOINTS_ENCODE_H_

#include <cstdint>

#include "astc/endpoints_quantize.h"
#include "astc/range.h"
#include "astc/vector.h"

int color_channel_sum(vec3i_t color) {
  return color.r + color.g + color.b;
}

void encode_luminance_direct(range_t endpoint_quant,
                             int v0,
                             int v1,
                             uint8_t endpoint_unquantized[2],
                             uint8_t endpoint_quantized[2]) {
  endpoint_quantized[0] = quantize_color(endpoint_quant, v0);
  endpoint_quantized[1] = quantize_color(endpoint_quant, v1);
  endpoint_unquantized[0] =
      unquantize_color(endpoint_quant, endpoint_quantized[0]);
  endpoint_unquantized[1] =
      unquantize_color(endpoint_quant, endpoint_quantized[1]);
}

bool encode_rgb_direct_blue_contract(range_t endpoint_quant,
                                     vec3f_t e0,
                                     vec3f_t e1,
                                     uint8_t endpoint_quantized[6],
                                     vec3i_t endpoint_unquantized[2]) {
  vec3f_t e0c(e0.r + e0.r - e0.b, e0.g + e0.g - e0.b, e0.b);
  vec3f_t e1c(e1.r + e1.r - e1.b, e1.g + e1.g - e1.b, e1.b);

  if (!is_rgb(e0c) || !is_rgb(e1c)) {
    return false;
  }

  vec3i_t e0q = quantize_color(endpoint_quant, round(e0c));
  vec3i_t e1q = quantize_color(endpoint_quant, round(e1c));
  vec3i_t e0u = unquantize_color(endpoint_quant, e0q);
  vec3i_t e1u = unquantize_color(endpoint_quant, e1q);

  if (color_channel_sum(e1u) <= color_channel_sum(e0u)) {
    return false;
  }

  endpoint_quantized[0] = static_cast<uint8_t>(e1q.r);
  endpoint_quantized[1] = static_cast<uint8_t>(e0q.r);
  endpoint_quantized[2] = static_cast<uint8_t>(e1q.g);
  endpoint_quantized[3] = static_cast<uint8_t>(e0q.g);
  endpoint_quantized[4] = static_cast<uint8_t>(e1q.b);
  endpoint_quantized[5] = static_cast<uint8_t>(e0q.b);

  // Can't use the unquantized values here since they are blue contracted.
  endpoint_unquantized[0] = round(e0);
  endpoint_unquantized[1] = round(e1);

  return true;
}

void encode_rgb_direct(range_t endpoint_quant,
                       vec3f_t e0,
                       vec3f_t e1,
                       uint8_t endpoint_quantized[6],
                       vec3i_t endpoint_unquantized[2]) {
  if (encode_rgb_direct_blue_contract(
          endpoint_quant, e0, e1, endpoint_quantized, endpoint_unquantized)) {
    return;
  }

  vec3i_t e0q = quantize_color(endpoint_quant, round(e0));
  vec3i_t e1q = quantize_color(endpoint_quant, round(e1));
  vec3i_t e0u = unquantize_color(endpoint_quant, e0q);
  vec3i_t e1u = unquantize_color(endpoint_quant, e1q);

  // ASTC uses a different blue contraction encoding when the sum of values for
  // the first endpoint is larger than the sum of values in the second
  // endpoint. Sort the endpoints to ensure that the normal encoding is used.
  if (color_channel_sum(e0u) > color_channel_sum(e1u)) {
    endpoint_quantized[0] = static_cast<uint8_t>(e1q.r);
    endpoint_quantized[1] = static_cast<uint8_t>(e0q.r);
    endpoint_quantized[2] = static_cast<uint8_t>(e1q.g);
    endpoint_quantized[3] = static_cast<uint8_t>(e0q.g);
    endpoint_quantized[4] = static_cast<uint8_t>(e1q.b);
    endpoint_quantized[5] = static_cast<uint8_t>(e0q.b);

    endpoint_unquantized[0] = e1u;
    endpoint_unquantized[1] = e0u;
  } else {
    endpoint_quantized[0] = static_cast<uint8_t>(e0q.r);
    endpoint_quantized[1] = static_cast<uint8_t>(e1q.r);
    endpoint_quantized[2] = static_cast<uint8_t>(e0q.g);
    endpoint_quantized[3] = static_cast<uint8_t>(e1q.g);
    endpoint_quantized[4] = static_cast<uint8_t>(e0q.b);
    endpoint_quantized[5] = static_cast<uint8_t>(e1q.b);

    endpoint_unquantized[0] = e0u;
    endpoint_unquantized[1] = e1u;
  }
}

#endif  // ASTC_ENDPOINTS_ENCODE_H_
