#include <iostream>
#include <string>

#include "src/commons.hpp"

int64_t ExtractMidDigits(int64_t number) {
    std::string str = std::to_string(number);
    int length = str.length();
    // Start index for extracting middle 4 digits
    int midStart = (length - 4) / 2;
    return std::stoll(str.substr(midStart, 4));
}

int64_t MidSquare(int64_t seed) {
    int64_t squared = seed * seed;
    int64_t mid = ExtractMidDigits(squared);
    std::cout << "MidSquare(" << seed << ")" << " -> " << squared << " -> " << mid << std::endl;
    return mid;
}

int main() {
    Header("Mid-Square Method");

    // using 4 digit of current time as seed
    int64_t seed = time(0) % 10000;

    for (int i = 0; i < 5; ++i) {
        seed = MidSquare(seed);
        std::cout << "[" << i + 1 << "] : " << seed << std::endl;
    }

    Footer();
}