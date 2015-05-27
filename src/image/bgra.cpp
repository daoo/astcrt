#include "image/bgra.hpp"

#include <stdint.h>
#include <stdio.h>

struct TGAHeader
{
  uint8_t id_length;
  uint8_t color_map_type;
  uint8_t image_type;

  uint8_t first_entry_index[2];
  uint8_t color_map_length[2];
  uint8_t color_map_entry_size;

  uint8_t origin_x[2], origin_y[2];
  uint8_t width[2], height[2];
  uint8_t pixel_depth;
  uint8_t image_descriptor;
};

void WriteTGAFile(const BgraImage& image, const char* file_path)
{
  FILE* file = fopen(file_path, "w");
  if (!file) {
    throw "could not open file for writing";
  }

  TGAHeader hdr;
  hdr.id_length = 0x00; // No id field
  hdr.color_map_type = 0x00; // No colormap
  hdr.image_type = 0x02; // Uncompressed true color

  // Color map unused, set to zero
  hdr.first_entry_index[0] = 0x00;
  hdr.first_entry_index[1] = 0x00;
  hdr.color_map_length[0] = 0x00;
  hdr.color_map_length[1] = 0x00;
  hdr.color_map_entry_size = 0x00;

  hdr.origin_x[0] = 0x00;
  hdr.origin_x[1] = 0x00;
  hdr.origin_y[0] = 0x00;
  hdr.origin_y[1] = 0x00;
  hdr.width[0] = image.width & 0xFF;
  hdr.width[1] = (image.width >> 8) & 0xFF;
  hdr.height[0] = image.height & 0xFF;
  hdr.height[1] = (image.height >> 8) & 0xFF;
  hdr.pixel_depth = 32;
  hdr.image_descriptor = 0x08;

  fwrite(&hdr, 1, sizeof(TGAHeader), file);
  fwrite(image.buffer, 1, image.buffer_size, file);
  fclose(file);
}

BgraImage ReadTGAFile(const char* file_path)
{
  FILE* file = fopen(file_path, "r");
  if (!file) {
    throw "could not open file for reading";
  }

  TGAHeader hdr;
  fread(&hdr, 1, sizeof(TGAHeader), file);

  if (hdr.color_map_type != 0x00) {
    fclose(file);
    throw "color map images not supported";
  }

  if (hdr.image_type != 0x02) {
    fclose(file);
    throw "only uncompressed true color supported";
  }

  if (((hdr.origin_x[1] << 8) | hdr.origin_x[0]) != 0) {
    fclose(file);
    throw "origin x should be 0";
  }

  if (((hdr.origin_y[1] << 8) | hdr.origin_y[0]) != 0) {
    fclose(file);
    throw "origin y should be 0";
  }

  if (hdr.pixel_depth != 24 && hdr.pixel_depth != 32) {
    fclose(file);
    throw "unsupported pixel depth";
  }

  if (hdr.pixel_depth == 24 && hdr.image_descriptor != 0) {
    fclose(file);
    throw "incorrect image descriptor for 24 bit pixel depth";
  }

  if (hdr.pixel_depth == 32 && hdr.image_descriptor != 0x08) {
    fclose(file);
    throw "incorrect image descriptor for 32 bit pixel depth";
  }

  fseek(file, hdr.id_length, SEEK_CUR);

  size_t width  = (hdr.width[1] << 8) | hdr.width[0];
  size_t height = (hdr.height[1] << 8) | hdr.height[0];

  BgraImage image(width, height);
  if (hdr.pixel_depth == 32) {
    fread(image.buffer, 1, image.buffer_size, file);
  }
  else if (hdr.pixel_depth == 24) {
    size_t byte_count = image.pixel_count*3;
    uint8_t* ptr = new uint8_t[byte_count];
    fread(ptr, 1, byte_count, file);
    for (size_t i = 0; i < image.pixel_count; ++i) {
      uint8_t* ptr_ = ptr + i*3;
      image.buffer[i].channels.r = ptr_[2];
      image.buffer[i].channels.g = ptr_[1];
      image.buffer[i].channels.b = ptr_[0];
      image.buffer[i].channels.a = 0xFF;
    }
  }

  fclose(file);

  return image;
}
