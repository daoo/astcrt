#!/usr/bin/env python

import bise
import datasize
import partitions
import quantize
import sys


def safe_head(lst, default):
    try:
        return lst[0]
    except:
        return default


def print_array_rec(file, element, fmt):
    if isinstance(element, int):
        file.write(fmt % element)
    elif isinstance(element, tuple) or isinstance(element, list):
        print_array_rec(file, iter(element), fmt)
    else:
        first = next(element)

        file.write("{")
        print_array_rec(file, first, fmt)
        for x in element:
            file.write(",")
            print_array_rec(file, x, fmt)
        file.write("}")


def print_bise_tables(file):
    trits_from_integer = bise.trits_from_integer_table()
    integer_from_trits = bise.integer_from_trits_table(trits_from_integer)
    quints_from_integer = bise.quints_from_integer_table()
    integer_from_quints = bise.integer_from_quints_table(quints_from_integer)

    file.write("const uint8_t trits_from_integer[256][5] = ")
    print_array_rec(file, iter(trits_from_integer), "%d")
    file.write(";\n\n")

    file.write("const uint8_t integer_from_trits[3][3][3][3][3] = ")
    print_array_rec(file, iter(integer_from_trits), "%d")
    file.write(";\n\n")

    file.write("const uint8_t quints_from_integer[128][3] = ")
    print_array_rec(file, iter(quints_from_integer), "%d")
    file.write(";\n\n")

    file.write("const uint8_t integer_from_quints[5][5][5] = ")
    print_array_rec(file, iter(integer_from_quints), "%d")
    file.write(";\n")


def print_partitions_tables(file):
    table = list(partitions.compute_partitioning_table(
            partition_count=2,
            block_width=4,
            block_height=4))

    file.write("const uint16_t partition_2_4x4_mask_table[1024] = ")
    print_array_rec(file, (part.partition_mask for part in table), "%#x")
    file.write(";\n\n")

    lookup_table = partitions.compute_partitioning_lookup_table(table)

    file.write("const int16_t partition_2_4x4_lookup_table[65536] = ")
    print_array_rec(
        file,
        (safe_head(partitions, -1) for partitions in lookup_table),
        "%#x")
    file.write(";\n")


def print_data_size_table(file, block_width, block_height):
    file.write("const int8_t color_endpoint_range_table[2][12][16] = ")
    print_array_rec(
        file,
        datasize.color_endpoint_range_table(block_width, block_height),
        "%d")
    file.write(";\n")


def print_color_quantization_tables(file):
    unquantize_table = quantize.color_unquantize_table()

    file.write("const uint8_t color_unquantize_table[21][256] = ")
    print_array_rec(file, iter(unquantize_table), "%d")
    file.write(";\n\n")

    file.write("const uint8_t color_quantize_table[21][256] = ")
    print_array_rec(
        file,
        iter(quantize.color_quantize_table(unquantize_table)),
        "%d")
    file.write(";\n")


def print_usage(prog):
    sys.stderr.write(
        ("Usage: %s COMMAND\n"
         "  Commands:\n"
         "    bise\n"
         "    partitions\n"
         "    datasize\n"
         "    quantize\n") % prog)


if len(sys.argv) == 1:
    print_usage(sys.argv[0])

try:
    i = 1
    while i < len(sys.argv):
        arg = sys.argv[i]

        if arg == "bise":
            print_bise_tables(sys.stdout)
        elif arg == "partitions":
            print_partitions_tables(sys.stdout)
        elif arg == "datasize":
            print_data_size_table(sys.stdout, 4, 4)
        elif arg == "quantize":
            print_color_quantization_tables(sys.stdout)
        else:
            raise ValueError("unknown mode " + arg)

        i = i+1

except ValueError as exception:
    sys.stderr.write("Error: %s\n" % exception)
    print_usage()
    sys.exit(1)
