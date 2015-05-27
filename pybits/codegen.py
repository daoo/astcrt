#!/usr/bin/env python

from bise import compute_bise_bitcount, trits_from_integer_table, integer_from_trits_table, quints_from_integer_table, integer_from_quints_table
from datasize import data_size, cem_values_count, color_endpoint_range, color_endpoint_range_table
from partitions import compute_partitioning_table, compute_partitioning_lookup_table, compute_partitioning_lookup_table_equality
from quantize import color_quantize_table, color_unquantize_table
from range import RANGE_CARDINALITY_TABLE
import sys

def safe_head(lst, default):
    try:
        return lst[0]
    except:
        return default

def print_array_rec(element, fmt):
    if isinstance(element, int):
        sys.stdout.write(fmt % element)
    elif isinstance(element, tuple) or isinstance(element, list):
        print_array_rec(iter(element), fmt)
    else:
        first = next(element)

        sys.stdout.write("{")
        print_array_rec(first, fmt)
        for x in element:
            sys.stdout.write(",")
            print_array_rec(x, fmt)
        sys.stdout.write("}")

def print_bise_tables():
    trits_from_integer  = trits_from_integer_table()
    integer_from_trits  = integer_from_trits_table(trits_from_integer)
    quints_from_integer = quints_from_integer_table()
    integer_from_quints = integer_from_quints_table(quints_from_integer)

    sys.stdout.write("const uint8_t trits_from_integer[256][5] = ")
    print_array_rec(iter(trits_from_integer), "%d")
    sys.stdout.write(";\n\n")

    sys.stdout.write("const uint8_t integer_from_trits[3][3][3][3][3] = ")
    print_array_rec(iter(integer_from_trits), "%d")
    sys.stdout.write(";\n\n")

    sys.stdout.write("const uint8_t quints_from_integer[128][3] = ")
    print_array_rec(iter(quints_from_integer), "%d")
    sys.stdout.write(";\n\n")

    sys.stdout.write("const uint8_t integer_from_quints[5][5][5] = ")
    print_array_rec(iter(integer_from_quints), "%d")
    sys.stdout.write(";\n")

def print_partitions_tables():
    table = list(compute_partitioning_table(
            partition_count = 2,
            block_width = 4,
            block_height = 4))

    sys.stdout.write("const uint16_t partition_2_4x4_mask_table[1024] = ")
    print_array_rec((part.partition_mask for part in table), "%#x")
    sys.stdout.write(";\n\n")

    lookup_table = compute_partitioning_lookup_table(table)

    sys.stdout.write("const int16_t partition_2_4x4_lookup_table[65536] = ")
    print_array_rec((safe_head(partitions, -1) for partitions in lookup_table), "%#x")
    sys.stdout.write(";\n")

def print_data_size_info(block_width, block_height, cem, partitions, weight_range):
    config_bits, weight_bits, remaining_bits = data_size(
            partitions = partitions,
            single_cem = True,
            block_width = block_width,
            block_height = block_height,
            block_depth = 1,
            dual_plane = False,
            weight_range = weight_range
        )

    ce_values = cem_values_count(cem, partitions)
    ce_range = color_endpoint_range(cem, remaining_bits, partitions)
    cem_bits = compute_bise_bitcount(ce_values, ce_range)

    print("block width:", block_width)
    print("block height:", block_height)
    print("config bits:", config_bits)
    print("weight count:", block_width * block_height)
    print("weight range:", RANGE_CARDINALITY_TABLE[weight_range])
    print("weight bits:", weight_bits)
    print("remaining bits:", remaining_bits)
    print("color endpoint values:", ce_values)
    print("color endpoint range:", RANGE_CARDINALITY_TABLE[ce_range])
    print("color endpoint bits:", cem_bits)
    print("unused bits:", remaining_bits - cem_bits)

def print_data_size_table(block_width, block_height):
    sys.stdout.write("const int8_t color_endpoint_range_table[2][12][16] = ")
    print_array_rec(color_endpoint_range_table(block_width, block_height), "%d")
    sys.stdout.write(";\n")

def print_color_quantization_tables():
    unquantize_table = color_unquantize_table()

    sys.stdout.write("const uint8_t color_unquantize_table[21][256] = ")
    print_array_rec(iter(unquantize_table), "%d")
    sys.stdout.write(";\n\n")

    sys.stdout.write("const uint8_t color_quantize_table[21][256] = ")
    print_array_rec(iter(color_quantize_table(unquantize_table)), "%d")
    sys.stdout.write(";\n")

def print_usage(prog):
    sys.stderr.write(
        ("Usage: %s COMMAND\n"
         "  Commands:\n"
         "    bise\n"
         "    partitions\n"
         "    datasize\n"
         "    quantize\n"
         "    data-info cem partitions weightrange\n") % prog)

if len(sys.argv) == 1:
    print_usage(sys.argv[0])

try:
    i = 1
    while i < len(sys.argv):
        arg = sys.argv[i]

        if arg == "bise":
            print_bise_tables()
        elif arg == "partitions":
            print_partitions_tables()
        elif arg == "datasize":
            print_data_size_table(4, 4)
        elif arg == "quantize":
            print_color_quantization_tables()
        elif arg == "data-info":
            print_data_size_info(
                    int(sys.argv[i+1]),
                    int(sys.argv[i+2]),
                    int(sys.argv[i+3]),
                    int(sys.argv[i+4]),
                    int(sys.argv[i+5]))
            i = i+5
        else:
            raise ValueError("unknown mode " + arg)

        i = i+1

except ValueError as exception:
    sys.stderr.write("Error: %s\n" % exception)
    print_usage()
    sys.exit(1)
