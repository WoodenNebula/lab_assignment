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
        fileName="${fileName%.*}"
    else
        fileNamePrefix="$(basename "$inputFile")"
        fileDir="$(dirname "$inputFile")"
        fileName="$(ls "$fileDir" | grep "${fileNamePrefix}")"

        if [[ -z "$fileName" ]]; then
            echo "File '$input' doesn't exist"
            exit 1
        fi

        src="$fileDir/$fileName"
    fi
}

compile() {
    local src="$1"
    local bin="$2"
    local isC="$3"

    if $isC; then
        bear -- gcc -g -I. "$src" -o "$bin"
    else
        bear -- g++ -std=c++23 -g -I. "$src" -o "$bin"
    fi || return 1

    chmod u+x "$bin"
}

run() {
    if [[ $# -eq 0 ]]; then
        echo "File path required!!!"
        return 1
    fi

    local file="${1#./}"
    make_src_path "$file"

    # extract extension
    local ext="${src##*.}"

    # make output folder
    local fdir="$(dirname "$src")"

    local binDir="${fdir#src/}"
    binDir="./bin/$binDir"

    local bin="$binDir/$fileName"

    mkdir -p "$binDir"

    # file type
    local isC=false
    if [[ "$ext" == "c" ]]; then
        isC=true
    fi

    # Compile, and don't run if compilation fails
    if ! compile "$src" "$bin" "$isC"; then
        echo "Compilation failed."
        return 1
    fi

    # Run by passing remaining arguments
    "$bin" "${@:2}"
    echo
}
run "$@"
