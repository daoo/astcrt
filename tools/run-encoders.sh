#!/usr/bin/env bash

# fastc etc1 uses rg-etc1
# fastc dxt1 uses intel code
# fastc dxt5 uses intel code
# nvtt is broken as it scales the image into square

set -eu

if [[ (( $# != 1 )) ]]; then
  echo "Usage: compare.sh IMAGE" 2>&1
  exit 1
fi

input="$1"

if [[ ! -f "$input" ]]; then
  echo "Error: image not found: $input" 2>&1
  exit 1
fi

dir="$HOME/tmp/compare"
name="$(basename "$input" .png)"
resized="$dir/${name}/${name}.resized"

mkdir -p "$dir"

[[ -f "${resized}.tga" ]] || make-divisible.sh "${input}" "${resized}.tga"
[[ -f "${resized}.png" ]] || make-divisible.sh "${input}" "${resized}.png"
[[ -f "${resized}-alpha.tga" ]] || make-divisible.sh --add-alpha "${input}" "${resized}-alpha.tga"
[[ -f "${resized}-alpha.png" ]] || make-divisible.sh --add-alpha "${input}" "${resized}-alpha.png"

do-astcrt() {
  echo "astcrt-$1"
  inp="${resized}-alpha.tga"
  enc="${resized}-alpha.encoded.astcrt-$1.astc"
  dec="${resized}-alpha.decoded.astcrt-$1.tga"
  [[ -f "$enc" ]] || astcrt-$1 "$inp" "$enc" > /dev/null
  [[ -f "$dec" ]] || astcenc -d "$enc" "$dec" -silentmode
}

do-astcenc() {
  echo "astcenc-${1}"
  inp="${resized}.tga"
  enc="${resized}.encoded.astcenc-${1}.astc"
  dec="${resized}.decoded.astcenc-${1}.tga"
  [[ -f "$enc" ]] || astcenc -c "$inp" "$enc" 4x4 -${1} -silentmode -j 4
  [[ -f "$dec" ]] || astcenc -d "$enc" "$dec" -silentmode
}

do-etcpak() {
  echo "etcpak"
  inp="${resized}.png"
  enc="${resized}.encoded.etcpak.pvr"
  dec="${resized}.decoded.etcpak.png"
  if [[ ! -f "$enc" ]]; then
    tmp="$(mktemp -d)"
    (cd "$tmp"; etcpak "$inp" -a -o 2 2>/dev/null)
    mv "$tmp/out.pvr" "$enc"
    mv "$tmp/out.png" "$dec"
    rm -rf "$tmp"
  fi
}

do-nvtt() {
  echo "nvtt-${1}"
  inp="${resized}.tga"
  enc="${resized}.encoded.nvtt-${1}.dds"
  dec="${resized}.deceded-${1}.tga"
  [[ -f "$enc" ]] || nvcompress -silent -${1} "$inp" "$enc"
  [[ -f "$dec" ]] || nvdecompress "$enc" "$dec" >/dev/null
}

do-fastc-bptc() {
  echo "fastc-bptc-q${1}"
  inp="${resized}.png"
  dec="${resized}.decoded.fastc-bptc-${1}.png"
  [[ -f "$dec" ]] || fastc -f BPTC -q ${1} -t 4 -d "$dec" "$inp" > /dev/null
}

do-etcpack() {
  echo "etcpack-${1}"
  inp="${resized}.tga"
  enc="${resized}.encoded.etcpack-${1}.pkm"
  dec="${resized}.decoded.etcpack-${1}.tga"
  if [[ ! -f "$dec" ]]; then
    tmp="$(mktemp -d)"
    file="$tmp/"
    etcpack "$inp" "$tmp" -c ${1} -f RGB -s slow -e nonperceptual > /dev/null
    mv "$tmp/$(basename "${resized}").pkm" "$enc"
    etcpack "$enc" "$tmp" -ext TGA > /dev/null
    mv "$tmp/$(basename "$enc" ".pkm").tga" "$dec"
    rm -rf "$tmp"
  fi
}

do-astcrt "linreg"
do-astcrt "partition"
do-astcenc "veryfast"
do-astcenc "fast"
do-astcenc "thorough"
do-astcenc "exhaustive"
do-etcpak
# do-nvtt "bc1"
# do-nvtt "bc7"
do-fastc-bptc 0
do-fastc-bptc 100
do-etcpack "etc1"
do-etcpack "etc2"
