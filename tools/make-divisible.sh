#!/usr/bin/env bash
#
# Resize an image to be evenly divisible into 4x4 blocks and optionally add an
# alpha channel.
#

set -eu

alpha=0
if [[ $# -eq 3 && ("$1" == "-a" || "$1" == "--add-alpha") ]]; then
  alpha=1
  shift
fi

if [[ ! $# -eq 2 ]]; then
  echo "Usage: $0 [-a|--add-alpha] INPUT OUTPUT" 1>&2
  exit 1
fi

input="$1"
output="$2"

if [[ ! -f "$input" ]]; then
  echo "Error: $input is not a file" 1>&2
  exit 1
fi

if [[ -f "$output" ]]; then
  echo "Warning: $output will be overwritten" 1>&2
fi

width=$(gm identify -format '%w' "$input")
height=$(gm identify -format '%h' "$input")

width_fix=$(((3 * width) % 4))
height_fix=$(((3 * height) % 4))

width_new=$((width + width_fix))
height_new=$((height + height_fix))

image_type="TrueColor"
if [[ $alpha -eq 1 ]]; then
  image_type="TrueColorMatte"
fi

gm convert $input \
  -type "$image_type" \
  -background white \
  -extent "${width_new}x${height_new}" \
  "$output"
