#!/bin/bash
file="$1"
src=""
bin=""
fileName=""


make_src_path() {
  local inputFile=$1

  if [[ "$inputFile" = *.* ]]; then
    src="$inputFile"
    fileName="$(basename -a $inputFile)"
  else
    fileNamePrefix="$(basename "$inputFile")"
    fileDir="$(dirname "$inputFile")"
    fileName="$(ls "$fileDir" | grep "${fileNamePrefix}" )"
    
    if [[ -z "$fileName" ]]; then
      echo "File '$input' doesn't exist"
      exit 1
    fi

    src="$fileDir/$fileName"
  fi
}

compile_and_run() {
    local src="$1"
    local bin="$2"
    local isC=$3

    # compile c/c++ file
    if $isC; then
      bear -- gcc -g -I. $src -o $bin
    else
      bear -- g++ -g -I. $src -o $bin
    fi
    # run
    chmod u+x $bin
    $bin
    echo
}             

run()  {
  if [ $# = 0 ]; then
    echo "File path required!!!"
  else
    file=${file#./}
    make_src_path "$file"
    
    # extract extension
    ext="${src##*.}"

    # make output folder
    fdir="$(dirname "$src")"

    binDir="${fdir#src/}"
    binDir="./bin/$binDir"

    bin="$binDir/$fileName"

    mkdir -p "$binDir"

    # file type
    isC=false
    if [[ "$ext" == "c" ]]; then
      isC=true
    else 
      isC=false
    fi

    compile_and_run $src $bin $isC
  fi
}

run $file
