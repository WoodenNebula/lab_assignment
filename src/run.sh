#!/bin/bash

file="$1"
src=""
bin=""
fileName=""

make_src_path() {
    local inputFile="$1"

    if [[ "$inputFile" == *.* ]]; then
        src="${inputFile#./}"
        fileName="$(basename "$src")"
        fileName="${fileName%.*}"
    else
        local fileNamePrefix
        local fileDir

        fileNamePrefix="$(basename "$inputFile")"
        fileDir="$(dirname "$inputFile")"

        fileName="$(ls "$fileDir" | grep "^${fileNamePrefix}\." | head -n 1)"

        if [[ -z "$fileName" ]]; then
            echo "File '$inputFile' doesn't exist"
            return 1
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

compile_flex() {
    local src="$1"
    local binDir="$2"
    local bin="$3"

    local baseName
    baseName="$(basename "$src" .l)"

    # Generate lex.yy.c directly into bin directory
    if ! flex -o "$binDir/${baseName}.yy.cpp" "$src"; then
        echo "Flex generation failed."
        return 1
    fi

    # Compile generated lexer
    if ! bear -- gcc -g -I. "$binDir/${baseName}.yy.cpp" -o "$bin"; then
        echo "Compilation failed."
        return 1
    fi

    chmod u+x "$bin"
}

compile_bison() {
    local src="$1"
    local binDir="$2"
    local bin="$3"

    local baseName
    baseName="$(basename "$src" .y)"

    # Generate parser.tab.c and parser.tab.h into bin directory
    if ! (
        cd "$binDir" &&
        bison -d -o "${baseName}.tab.cpp" "$OLDPWD/$src"
    ); then
        echo "Bison generation failed."
        return 1
    fi

    # Compile generated parser
    if ! bear -- g++ -std=c++23 -g -I. \
        "$binDir/${baseName}.tab.cpp" \
        -o "$bin"; then
        echo "Compilation failed."
        return 1
    fi

    chmod u+x "$bin"
}

compile_flex_bison() {
    local lexSrc="$1"
    local yaccSrc="$2"
    local binDir="$3"
    local bin="$4"

    local yaccBase
    yaccBase="$(basename "$yaccSrc" .y)"

    # ------------------------------------------------------------
    # Bison
    # ------------------------------------------------------------

    if ! (
        cd "$binDir" &&
        bison -d -o "${yaccBase}.tab.cpp" "$OLDPWD/$yaccSrc"
    ); then
        echo "Bison generation failed."
        return 1
    fi

    # ------------------------------------------------------------
    # Flex
    # ------------------------------------------------------------

    local lexBase
    lexBase="$(basename "$lexSrc" .l)"

    if ! flex -o "$binDir/${lexBase}.yy.cpp" "$lexSrc"; then
        echo "Flex generation failed."
        return 1
    fi

    # ------------------------------------------------------------
    # Compile lexer + parser
    # ------------------------------------------------------------

    if ! bear -- g++ -std=c++23 -g -I. \
        "$binDir/${yaccBase}.tab.cpp" \
        "$binDir/${lexBase}.yy.cpp" \
        -o "$bin"; then
        echo "Compilation failed."
        return 1
    fi

    chmod u+x "$bin"
}

run() {
    if [[ $# -eq 0 ]]; then
        echo "File path required!!!"
        return 1
    fi

    local lexFile=""
    local yaccFile=""
    local normalFile=""
    local -a programArgs=()

    # ------------------------------------------------------------
    # Identify source files
    # ------------------------------------------------------------

    for arg in "$@"; do
        case "${arg##*.}" in
            l)
                if [[ -n "$lexFile" ]]; then
                    echo "Error: multiple .l files specified."
                    return 1
                fi

                lexFile="${arg#./}"
                ;;

            y)
                if [[ -n "$yaccFile" ]]; then
                    echo "Error: multiple .y files specified."
                    return 1
                fi

                yaccFile="${arg#./}"
                ;;

            *)
                if [[ -z "$normalFile" ]]; then
                    normalFile="${arg#./}"
                else
                    programArgs+=("$arg")
                fi
                ;;
        esac
    done

    # ============================================================
    # Flex / Bison
    # ============================================================

    if [[ -n "$lexFile" || -n "$yaccFile" ]]; then

        # --------------------------------------------------------
        # Validate source files
        # --------------------------------------------------------

        if [[ -n "$lexFile" && ! -f "$lexFile" ]]; then
            echo "File '$lexFile' doesn't exist"
            return 1
        fi

        if [[ -n "$yaccFile" && ! -f "$yaccFile" ]]; then
            echo "File '$yaccFile' doesn't exist"
            return 1
        fi

        # --------------------------------------------------------
        # Determine output path from source path
        #
        # src/foo/bar/lexer.l
        #       ↓
        # bin/foo/bar/lexer
        # --------------------------------------------------------

        local sourceFile
        local sourceDir
        local relativeDir

        if [[ -n "$yaccFile" ]]; then
            sourceFile="$yaccFile"
        else
            sourceFile="$lexFile"
        fi

        fileName="$(basename "$sourceFile")"
        fileName="${fileName%.*}"

        sourceDir="$(dirname "$sourceFile")"
        relativeDir="${sourceDir#src/}"

        local binDir="./bin/$relativeDir"
        local bin="$binDir/$fileName"

        mkdir -p "$binDir"

        # --------------------------------------------------------
        # Flex + Bison
        # --------------------------------------------------------

        if [[ -n "$lexFile" && -n "$yaccFile" ]]; then
            echo "Bison: $yaccFile"
            echo "Flex:  $lexFile"

            if ! compile_flex_bison \
                "$lexFile" \
                "$yaccFile" \
                "$binDir" \
                "$bin"; then
                return 1
            fi

        # --------------------------------------------------------
        # Flex only
        # --------------------------------------------------------

        elif [[ -n "$lexFile" ]]; then
            echo "Flex:   $lexFile"

            if ! compile_flex "$lexFile" "$binDir" "$bin"; then
                return 1
            fi

        # --------------------------------------------------------
        # Bison only
        # --------------------------------------------------------

        elif [[ -n "$yaccFile" ]]; then
            echo "Bison:  $yaccFile"

            if ! compile_bison "$yaccFile" "$binDir" "$bin"; then
                return 1
            fi
        fi

        # Run executable
        "$bin" "${programArgs[@]}"
        return $?
    fi

    # ============================================================
    # Normal C/C++
    # ============================================================

    local file="${normalFile#./}"

    if [[ -z "$file" ]]; then
        echo "Source file required."
        return 1
    fi

    make_src_path "$file" || return 1

    local ext="${src##*.}"
    local fdir
    fdir="$(dirname "$src")"

    local binDir="${fdir#src/}"
    binDir="./bin/$binDir"

    bin="$binDir/$fileName"

    mkdir -p "$binDir"

    local isC=false

    if [[ "$ext" == "c" ]]; then
        isC=true
    fi

    if ! compile "$src" "$bin" "$isC"; then
        echo "Compilation failed."
        return 1
    fi

    "$bin" "${programArgs[@]}"
    echo
}

run "$@"