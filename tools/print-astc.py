#!/usr/bin/env python3

import sys

# Utility that prints the blocks of an ASTC file in binary format.

# TODO: Basic decoding to generate nicer output?

# Represent a number as a binary string.
def binary(num, count):
    assert isinstance(num, int)
    bits = []
    for i in range(count):
        bits.append('1' if (num >> (count-i-1)) & 1 else '0')
    return ''.join(bits)

# Print the header with bit indices.
def print_table_heading():
    # First row: bit indices
    sys.stdout.write(" " * 4)
    sys.stdout.write("|")
    for byte in range(16):
        msb  = byte*8+7
        lsb  = byte*8
        sys.stdout.write(str(lsb).ljust(4, "."))
        sys.stdout.write(str(msb).rjust(4, "."))
        sys.stdout.write(" ")
    sys.stdout.write("\n")

    # Second row: a line
    sys.stdout.write("-" * 4)
    sys.stdout.write("|")
    sys.stdout.write("-" * (128 + 16 - 1))
    sys.stdout.write("\n")

# Print the column labels.
def print_column(i):
    sys.stdout.write(str(i).ljust(4, " "))
    sys.stdout.write("|")

def print_header(header):
    if header[0:4] == b'\x13\xAB\xA1\x5C':
        xblock = header[4]
        yblock = header[5]
        zblock = header[6]

        xsize = int.from_bytes(header[7:10], byteorder='little')
        ysize = int.from_bytes(header[10:13], byteorder='little')
        zsize = int.from_bytes(header[13:16], byteorder='little')

        print("Block dimension {}x{}x{}".format(xblock, yblock, zblock))
        print("Image dimension {}x{}x{}".format(xsize, ysize, zsize))
        print()

        return True
    return False

def print_block(index, block):
    print_column(index)
    for byte in block:
        sys.stdout.write(binary(byte, 8))
        sys.stdout.write(' ')
    sys.stdout.write('\n')

if len(sys.argv) != 2:
    sys.stderr.write("Usage: {} FILE\n".format(sys.argv[0]))
    sys.exit(1)

file_path = sys.argv[1]

with open(file_path, "rb") as file:
    header = file.read(16)
    if print_header(header):
        print_table_heading()

        index = 0
        while True:
            block = file.read(16)
            if block and len(block) == 16:
                print_block(index, block)
                index = index + 1
            else:
                break
