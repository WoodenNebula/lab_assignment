/*
program to read an input string and classify it as:
⎯ Reserved Keyword (matches standard C keywords).
⎯ Valid Identifier (follows rules: starts with letter/underscore, containing alphanumeric/underscore characters).
⎯ Invalid Identifier (prints exact rule violated).
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

constexpr std::array<std::string_view, 40> KEYWORDS = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for",
    "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned",
    "void", "volatile", "while", "decltype", "default", "delete",
};

void ValidateIdentifierOrKeywords(std::string_view input) {
    // Check if the input is a reserved keyword
    if (std::find(KEYWORDS.begin(), KEYWORDS.end(), input) != KEYWORDS.end()) {
        std::println("{}'{}' is a reserved keyword{}", GREEN, input, RESET);
    }
    else {
        // Check if the input is a valid identifier
        bool isValidIdentifier = true;
        if (input.empty() || (!std::isalpha(input[0]) && input[0] != '_')) {
            isValidIdentifier = false;
            std::println("{}INVALID IDENTIFIER '{}': Identifier must start with a letter or underscore{}", RED, input, RESET);
        }
        else {
            for (char c : input) {
                if (!std::isalnum(c) && c != '_') {
                    isValidIdentifier = false;
                    std::println("{}INVALID IDENTIFIER '{}': Identifier contains invalid character '{}', Only alphanumeric characters and underscores are allowed{}", RED, input, c, RESET);
                    break;
                }
            }
        }

        if (isValidIdentifier) {
            std::println("{}'{}' is a valid identifier{}", GREEN, input, RESET);
        }
    }
}

void Tests() {
    std::vector<std::string> testInputs = {
        "int", "myVariable", "2ndVar", "_privateVar", "var$", "while", "valid_identifier", "invalid-identifier"
    };

    for (const auto& input : testInputs) {
        std::print("Testing input: '{}' -> ", input);
        ValidateIdentifierOrKeywords(input);
    }
}

int main() {
    Header("Keyword and Identifier Classifier");

    // std::string input = GetInput();
    // ValidateIdentifierOrKeywords(input);

    Tests();

    Footer();
}