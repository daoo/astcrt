#ifndef DATA_SIZE_H_LF5Z8OQB
#define DATA_SIZE_H_LF5Z8OQB

#include <stddef.h>
#include <stdint.h>

#include "astc/data_size_table.hpp"
#include "astc/dcheck.hpp"
#include "astc/endpoints.hpp"
#include "astc/range.hpp"

range_t endpoint_quantization(size_t partitions,
                              range_t weight_quant,
                              color_endpoint_mode_t endpoint_mode) {
  int8_t ce_range =
      color_endpoint_range_table[partitions - 1][weight_quant][endpoint_mode];
  DCHECK(ce_range >= 0 && ce_range <= RANGE_MAX);
  return static_cast<range_t>(ce_range);
}

#endif /* end of include guard: DATA_SIZE_H_LF5Z8OQB */
