/* Algorithm (Euclidean Algorithm for GCD):
1. Start
2. Given two positive integers a and b.
3. While b != 0:
   a. r = a mod b
   b. a = b
   c. b = r
4. When loop ends, a is the GCD.
5. End
*/

#include <iostream>

#include "src/commons.hpp"

int gcdEuclidean(int a, int b) {
    while (b != 0) {
        int r = a % b;
        a = b;
        b = r;
    }

    return a;
}

int main() {
    Header("EUCLIDEAN GCD");

    int a = 252;
    int b = 105;

    std::cout << "a = " << a << ", b = " << b << std::endl;
    std::cout << "GCD(" << a << ", " << b << ") = " << gcdEuclidean(a, b)
        << std::endl;

    Footer();
    return 0;
}
