#include <iostream>

#include "src/commons.hpp"

int64_t RandomLCM(int64_t a, int64_t c, int64_t m, int64_t seed) {
    std::cout << "Calculating random number using LCM with parameters: a=" << a << ", c=" << c << ", m=" << m << ", seed=" << seed << std::endl;
    return (a * seed + c) % m;
}

int main() {
    Header("Linear Congruential Method");

    int64_t a = 16625;
    int64_t c = 904223;
    int64_t m = 1000;
    int64_t seed = time(0);

    for (int i = 0; i < 5; ++i) {
        seed = RandomLCM(a, c, m, seed);
        std::cout << "[" << i + 1 << "] : " << seed << std::endl;
    }

    Footer();
}