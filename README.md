astcrt
======

This repository contains the code written for my thesis: Real-Time Adaptive
Scalable Texture Compression for the Web[1]. It contains the program `astcrt`,
which compress input images into ASTC[2] files.

Building
--------

To build run `./build/configure && ninja -C out_clang_debug`.

Dependencies:

  * Bash
  * Ninja
  * Clang (for debug builds)
  * GCC (for release builds)
  * GCC with ARM cross compiler (for arm builds)

Running
-------

The `astcrt` program encodes an TGA file using the ASTC format. The program
takes two arguments: input TGA file path and output ASTC file path. The TGA
file must be uncompressed 32-bit BGRA without a color map. The width and height
of the input file must also be a multiple of 4. Prepare the image with an image
manipulation program.

Decoding
--------

The reference implementation by ARM[3] can be used for testing the decoder.

Run it with the following commands:

    astcrt original.tga encoded.astc
    astcenc -d encoded.astc decoded.tga
    astcenc -compare original.tga decode.tga -showpsnr


References
----------

1. [Real-Time Adaptive Scalable Texture Compression for the Web](http://studentarbeten.chalmers.se/publication/234933-real-time-adaptive-scalable-texture-compression-for-the-web)
2. [Adaptive Scalable Texture Compression](https://en.wikipedia.org/wiki/Adaptive_Scalable_Texture_Compression)
3. [Reference Encoder](https://github.com/ARM-software/astc-encoder)
