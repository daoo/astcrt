#!/usr/bin/env python3

import os
import subprocess
import sys

def peak_signal_to_noise_ratio(a, b):
    result = subprocess.run(["astcenc", "-compare", a, b, "-showpsnr"], stdout=subprocess.PIPE)
    for line in result.stdout.decode("utf-8").splitlines():
        parts = line.split()
        if parts[0] == "PSNR" and parts[1] == "(LDR-RGB):":
            return float(parts[2])

    raise RuntimeError("unexpected output from astcenc")

def collect_results(paths):
    results = {}
    for root_path in paths:
        for file_name in os.listdir(root_path):
            file_path = os.path.join(root_path, file_name)
            parts = file_name.split(".")

            if len(parts) == 5:
                [image_name, resized, state, program, file_type] = parts
                if state == "decoded":
                    decoded_path = file_path
                    original_name = ".".join([image_name, resized, file_type])
                    original_path = os.path.join(root_path, original_name)

                    psnr = peak_signal_to_noise_ratio(original_path, decoded_path)
                    try:
                        results[image_name][program] = psnr
                    except:
                        results[image_name] = { program: psnr }
    return results

def collect_programs(results):
    programs = set()
    for result in results.values():
        for program in result.keys():
            programs.add(program)
    return sorted(programs)

def output_results(handle, programs, results):
    handle.write("file,")
    handle.write(",".join(programs))
    handle.write("\n")

    for image in results.keys():
        handle.write(image)
        for program in programs:
            try:
                handle.write(",%f" % results[image][program])
            except KeyError:
                handle.write(",")
        handle.write("\n")

def main(paths):
    results = collect_results(paths)
    programs = collect_programs(results)
    output_results(sys.stdout, programs, results)

if len(sys.argv) > 1:
    main(sys.argv[1:])
else:
    sys.stderr.write("Usage: %s DIRECTORY...\n" % sys.argv[0])
