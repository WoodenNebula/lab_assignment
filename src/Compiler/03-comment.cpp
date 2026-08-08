/*
program to identify whether a given line is a comment or not
⎯ Read the input string.
⎯ Check whether the string is single or multiline comment. (Hint: string containing '/' and check next character is '/'or '*' and ending with ‘*’ followed by ‘/’)
⎯ If condition satisfies print “comment”.
⎯ Else print “not a comment”.
*/

#include "src/commons.hpp"

#include <print>
#include <string>

std::string GetInput() {
    std::string input;
    std::print("Enter a string: ");
    std::getline(std::cin, input);
    return input;
}

bool isSingleLineComment(std::string_view str) {
    if (str.length() < 2) return false;
    return str[0] == '/' && str[1] == '/';
}

bool isMultiLineComment(std::string_view str) {
    if (str.length() < 4) return false;
    return str[0] == '/' && str[1] == '*' && str[str.length() - 2] == '*' && str[str.length() - 1] == '/';
}

int main() {
    Header("Comment Identifier");

    std::string input = GetInput();

    if (isSingleLineComment(input)) {
        std::println("Single-line comment");
    }
    else if (isMultiLineComment(input)) {
        std::println("Multi-line comment");
    }
    else {
        std::println("Not a comment");
    }

    Footer();
}