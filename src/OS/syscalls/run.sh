#!/bin/bash
fname="$1"
fdir="./src/OS"
bin="./src/OS/outputs/"

if [ $# = 0 ]; then
  echo "File name required!!!"
else
  if [ -n "$SRC_DIR" ]; then
    fdir="$fdir/$SRC_DIR/"
    fbin="$bin$1"
    file="$fdir$1.c"
    # echo "file=$file, fbin=$fbin"
    gcc $file -o $fbin
    chmod u+x $fbin
    $fbin
    echo
  else 
    echo "SRC_DIR environment variable not set, please set it to the sub directory of OS/ from where the .c file is sourced"
  fi
fi
