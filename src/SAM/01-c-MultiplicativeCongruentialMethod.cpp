#include <iostream>

#include "src/commons.hpp"

int64_t RandomMCM(int64_t a, int64_t m, int64_t seed) {
    std::cout << "Calculating random number using MCM with parameters: a=" << a << ", m=" << m << ", seed=" << seed << std::endl;
    return (a * seed) % m;
}

int main() {
    Header("Multiplicative Congruential Method");

    int64_t a = 16625;
    int64_t m = 1000;
    int64_t seed = time(0);

    for (int i = 0; i < 5; ++i) {
        seed = RandomMCM(a, m, seed);
        std::cout << "[" << i + 1 << "] : " << seed << std::endl;
    }

    Footer();
}