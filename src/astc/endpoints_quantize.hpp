#ifndef COLOR_QUANTIZATION_H_4MSDGEZD
#define COLOR_QUANTIZATION_H_4MSDGEZD

#include <stdint.h>

#include "astc/dcheck.hpp"
#include "astc/endpoint_quantize_tables.hpp"
#include "astc/range.hpp"
#include "astc/vector.hpp"

uint8_t quantize_color(range_t quant, int c) {
  DCHECK(c >= 0 && c <= 255);
  return color_quantize_table[quant][c];
}

vec3i_t quantize_color(range_t quant, vec3i_t c) {
  vec3i_t result;
  result.r = color_quantize_table[quant][c.r];
  result.g = color_quantize_table[quant][c.g];
  result.b = color_quantize_table[quant][c.b];
  return result;
}

uint8_t unquantize_color(range_t quant, int c) {
  DCHECK(c >= 0 && c <= 255);
  return color_unquantize_table[quant][c];
}

vec3i_t unquantize_color(range_t quant, vec3i_t c) {
  vec3i_t result;
  result.r = color_unquantize_table[quant][c.r];
  result.g = color_unquantize_table[quant][c.g];
  result.b = color_unquantize_table[quant][c.b];
  return result;
}

#endif /* end of include guard: COLOR_QUANTIZATION_H_4MSDGEZD */
