#include "image/compressed.h"

#include <cstdint>
#include <cstdio>

struct CompressedHeader {
  uint8_t magic[4];
  uint8_t xdim;
  uint8_t ydim;
  uint8_t xsize[3];
  uint8_t ysize[3];
  uint8_t bytes_per_block;
};

void WriteCompressedFile(uint32_t magic,
                         const CompressedImage& compressed,
                         const char* file_path) {
  FILE* file = fopen(file_path, "w");
  if (!file) {
    throw "could not open file for writing";
  }

  CompressedHeader hdr;
  hdr.magic[0] = static_cast<uint8_t>(magic & 0xFF);
  hdr.magic[1] = static_cast<uint8_t>((magic >> 8) & 0xFF);
  hdr.magic[2] = static_cast<uint8_t>((magic >> 16) & 0xFF);
  hdr.magic[3] = static_cast<uint8_t>((magic >> 24) & 0xFF);
  hdr.xdim = static_cast<uint8_t>(compressed.xdim);
  hdr.ydim = static_cast<uint8_t>(compressed.ydim);
  hdr.xsize[0] = compressed.xsize & 0xFF;
  hdr.xsize[1] = (compressed.xsize >> 8) & 0xFF;
  hdr.xsize[2] = (compressed.xsize >> 16) & 0xFF;
  hdr.ysize[0] = compressed.ysize & 0xFF;
  hdr.ysize[1] = (compressed.ysize >> 8) & 0xFF;
  hdr.ysize[2] = (compressed.ysize >> 16) & 0xFF;
  hdr.bytes_per_block = static_cast<uint8_t>(compressed.bytes_per_block);

  fwrite(&hdr, 1, sizeof(CompressedHeader), file);
  fwrite(compressed.buffer, 1, compressed.buffer_size, file);
  fclose(file);
}

CompressedImage ReadCompressedFile(uint32_t magic, const char* file_path) {
  FILE* file = fopen(file_path, "r");
  if (!file) {
    throw "could not open file for reading";
  }

  CompressedHeader hdr;
  fread(&hdr, 1, sizeof(CompressedHeader), file);

  uint32_t magic_ = (hdr.magic[3] << 24) | (hdr.magic[2] << 16) |
                    (hdr.magic[1] << 8) | (hdr.magic[0]);

  if (magic != magic_) {
    fclose(file);
    throw "incorrect magic number in header";
  }

  size_t xsize = (hdr.xsize[2] << 16) | (hdr.xsize[1] << 8) | hdr.xsize[0];
  size_t ysize = (hdr.ysize[2] << 16) | (hdr.ysize[1] << 8) | hdr.ysize[0];

  CompressedImage compressed(xsize, ysize, hdr.xdim, hdr.ydim,
                             hdr.bytes_per_block);
  fread(compressed.buffer, 1, compressed.buffer_size, file);
  fclose(file);

  return compressed;
}
