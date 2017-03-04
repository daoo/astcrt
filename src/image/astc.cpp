#include "image/astc.hpp"
#include "image/compressed.hpp"

#include <stdint.h>
#include <stdio.h>

static int MAGIC_FILE_CONSTANT = 0x5CA1AB13;

struct ASTCHeader {
  uint8_t magic[4];
  uint8_t blockdim_x;
  uint8_t blockdim_y;
  uint8_t blockdim_z;
  uint8_t xsize[3];
  uint8_t ysize[3];
  uint8_t zsize[3];
};

void WriteASTCFile(const CompressedImage& compressed, const char* file_path) {
  FILE* file = fopen(file_path, "w");
  if (!file) {
    throw "could not open file for writing";
  }

  ASTCHeader hdr;
  hdr.magic[0] = static_cast<uint8_t>(MAGIC_FILE_CONSTANT & 0xFF);
  hdr.magic[1] = static_cast<uint8_t>((MAGIC_FILE_CONSTANT >> 8) & 0xFF);
  hdr.magic[2] = static_cast<uint8_t>((MAGIC_FILE_CONSTANT >> 16) & 0xFF);
  hdr.magic[3] = static_cast<uint8_t>((MAGIC_FILE_CONSTANT >> 24) & 0xFF);
  hdr.blockdim_x = static_cast<uint8_t>(compressed.xdim);
  hdr.blockdim_y = static_cast<uint8_t>(compressed.ydim);
  hdr.blockdim_z = 1;
  hdr.xsize[0] = compressed.xsize & 0xFF;
  hdr.xsize[1] = (compressed.xsize >> 8) & 0xFF;
  hdr.xsize[2] = (compressed.xsize >> 16) & 0xFF;
  hdr.ysize[0] = compressed.ysize & 0xFF;
  hdr.ysize[1] = (compressed.ysize >> 8) & 0xFF;
  hdr.ysize[2] = (compressed.ysize >> 16) & 0xFF;
  hdr.zsize[0] = 1 & 0xFF;
  hdr.zsize[1] = (1 >> 8) & 0xFF;
  hdr.zsize[2] = (1 >> 16) & 0xFF;

  fwrite(&hdr, 1, sizeof(ASTCHeader), file);
  fwrite(compressed.buffer, 1, compressed.buffer_size, file);
  fclose(file);
}
