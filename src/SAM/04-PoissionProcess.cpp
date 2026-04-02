#include <iostream>
#include <cmath>
#include <cstdlib>

#include "src/commons.hpp"

int PoissionRandomVariate(double lambda) {
    double L = std::exp(-lambda);
    int numberOfIteration = 0;
    double probabilityProduct = 1.0;

    // loop until the product of random numbers is less than or equal to L
    // the number of iterations - 1 will give us the number of arrivals in the Poisson process
    do {
        numberOfIteration++;

        // Generate a random number between 0 and 1
        double randomNum = static_cast<double>(rand()) / RAND_MAX;

        probabilityProduct *= randomNum;
    } while (probabilityProduct > L);

    return numberOfIteration - 1;
}

int main() {
    Header("Poisson Process Simulation");

    int seed = 80010139;

    srand(seed);

    // average rate of arrival
    double lambda = 0.9;

    for (int i = 0; i < 10; i++) {
        std::cout << "Random number of arrivals: " << PoissionRandomVariate(lambda) << std::endl;
    }

    Footer();
}
