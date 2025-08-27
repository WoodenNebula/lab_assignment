#!/bin/bash
file="$1"
bin="./bin"

if [ $# = 0 ]; then
  echo "File path required!!!"
else
  # setup output folder
  fdir=$(dirname "$file")
  fbinPath="$bin/${fdir#./src/}"
  mkdir -p "$fbinPath"

    ext=${file##*.} # get the extension
    filePathNoExt=${file%.*}
    filePathNoExtNoSrc=${filePathNoExt#./src/}
    fbin="$bin/$filePathNoExtNoSrc"


    # compile c/c++ file
    if [[ "$ext" == "c" ]]; then
      bear -- gcc -g -I. $file -o $fbin
    else
      bear -- g++ -g -I. $file -o $fbin
    fi
    # run
    chmod u+x $fbin
    $fbin
    echo
fi
