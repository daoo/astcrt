#!/usr/bin/env bash

set -eu

astcenc-veryfast() {
  taskset --cpu-list 0 astcenc -c "$1" /dev/null 4x4 -j 1 -veryfast -silentmode -time
}

astcenc-thorough() {
  taskset --cpu-list 0 astcenc -c "$1" /dev/null 4x4 -j 1 -thorough -silentmode -time
}

astcrt-linreg() {
  taskset --cpu-list 0 astcrt-linreg "$1" /dev/null
}

astcrt-partition() {
  taskset --cpu-list 0 astcrt-partition "$1" /dev/null
}

fastc-bptc-q0() {
  taskset --cpu-list 0 fastc -nd -f BPTC -t 1 -n 1 -q 0 "$1"
}

fastc-bptc-q100() {
  taskset --cpu-list 0 fastc -nd -f BPTC -t 1 -n 1 -q 100 "$1"
}

etcpak() {
  taskset --cpu-list 0 etcpak "$1" -b
}

bench() {
  preiterations="$1"
  shift
  iterations=$1
  shift
  tool="$1"
  shift
  filetype="$1"
  shift
  images=$*

  echo "$tool"
  for image in $images; do
    image="$(basename $image)"
    file="$image/${image}.${filetype}"
    output="$HOME/tmp/${image}-${tool}.txt"

    echo "$image"
    for i in $(seq $preiterations); do
      $tool "$file"
    done > /dev/null
    for i in $(seq $iterations); do
      $tool "$file"
    done > "$output"
  done
}

images=$@

bench 10 50 astcrt-linreg "resized-alpha.tga" $images
bench 10 50 astcrt-partition "resized-alpha.tga" $images
bench 1  50 astcenc-veryfast "resized.tga" $images
bench 0  1  astcenc-thorough "resized.tga" $images
bench 10 50 etcpak "resized.png" $images
bench 1  10 fastc-bptc-q0 "resized.png" $images
bench 1  1  fastc-bptc-q100 "resized.png" $images
