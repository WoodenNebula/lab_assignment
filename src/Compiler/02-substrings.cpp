/*
Implement a program that accepts a string and generates:
- all proper and improper prefixes
- all proper and improper suffixes
- all possible substrings
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

int main() {
    Header("Substring Generator");

    std::string input = GetInput();

    std::println("Proper and Improper Prefixes:");
    for (size_t i = 0; i <= input.length(); ++i) {
        std::print("\t - {}\n", input.substr(0, i));
    }

    std::println("\nProper and Improper Suffixes:");
    for (size_t i = 0; i <= input.length(); ++i) {
        std::print("\t - {}\n", input.substr(i));
    }

    std::println("\nAll Possible Substrings:");
    for (size_t i = 0; i < input.length(); ++i) {
        for (size_t j = 1; j <= input.length() - i; ++j) {
            std::print("\t - {}\n", input.substr(i, j));
        }
    }
    Footer();
}