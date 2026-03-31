#include <iostream>
#include <cstdlib>

#include "src/commons.hpp"

int main() {
    Header("Simple Random Number Generator");

    int seed = time(0);
    srand(seed);

    std::cout << "Seeded( " << seed << " ) Random number is : " << rand() << std::endl;

    Footer();
}