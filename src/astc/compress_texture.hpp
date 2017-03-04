#ifndef COMPRESS_TEXTURE_H_VYGPWUPA
#define COMPRESS_TEXTURE_H_VYGPWUPA

#include <stdint.h>

/**
 * Compress an texture with the ASTC format.
 *
 * @param src The source data, width*height*4 bytes with BGRA ordering.
 * @param dst The output, width*height bytes.
 * @param width The width of the input texture.
 * @param height The height of the input texture.
 */
void compress_texture(const uint8_t* src, uint8_t* dst, int width, int height);

#endif /* end of include guard: COMPRESS_TEXTURE_H_VYGPWUPA */
