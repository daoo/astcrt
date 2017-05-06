#ifndef ASTC_DATA_SIZE_H_
#define ASTC_DATA_SIZE_H_

#include <cstddef>
#include <cstdint>

#include "astc/data_size_table.h"
#include "astc/dcheck.h"
#include "astc/endpoints.h"
#include "astc/range.h"

range_t endpoint_quantization(size_t partitions,
                              range_t weight_quant,
                              color_endpoint_mode_t endpoint_mode) {
  int8_t ce_range =
      color_endpoint_range_table[partitions - 1][weight_quant][endpoint_mode];
  DCHECK(ce_range >= 0 && ce_range <= RANGE_MAX);
  return static_cast<range_t>(ce_range);
}

#endif  // ASTC_DATA_SIZE_H_
