#ifndef ENDPOINTS_PRINCIPAL_COMPONENTS_HPP_XTRHPOBI
#define ENDPOINTS_PRINCIPAL_COMPONENTS_HPP_XTRHPOBI

#include <stddef.h>

#include "astc/colors.h"
#include "astc/constants.h"
#include "astc/vector.h"

void principal_component_analysis(const unorm8_t texels[BLOCK_TEXEL_COUNT],
                                  size_t count,
                                  vec3f_t& line_k,
                                  vec3f_t& line_m);

inline void principal_component_analysis_block(
    const unorm8_t texels[BLOCK_TEXEL_COUNT],
    vec3f_t& line_k,
    vec3f_t& line_m) {
  principal_component_analysis(texels, BLOCK_TEXEL_COUNT, line_k, line_m);
}

#endif /* end of include guard: ENDPOINTS_PRINCIPAL_COMPONENTS_HPP_XTRHPOBI */
