#!/bin/bash
fname="$1"
fdir="./src/OS"
bin="./src/OS/outputs/"

if [ $# = 0 ]; then
  echo "File name required!!!"
else
  if [ $# = 2 ]; then
    fdir="$fdir/threads/"
  else
    fdir="$fdir/syscalls/"
  fi

  fbin="$bin$1"
  file="$fdir$1.c"
  # echo "file=$file, fbin=$fbin"
  gcc $file -o $fbin
  chmod u+x $fbin
  $fbin
  echo
fi
