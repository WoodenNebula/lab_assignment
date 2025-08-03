#!/bin/bash
fname="$1"
fdir="./src/OS/syscalls/"
bin="./src/OS/outputs/"

if [ $# = 0 ]; then
  echo "File name required!!!"
else
  fbin="$bin$1"
  file="$fdir$1.c"
  # echo "file=$file, fbin=$fbin"
  gcc $file -o $fbin
  chmod u+x $fbin
  $fbin
  echo
fi
