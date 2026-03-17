/* Algorithm (Check Co-Prime Numbers using GCD):
1. Start
2. Compute GCD(a, b) using Euclidean algorithm.
3. If GCD(a, b) == 1, then numbers are co-prime.
4. Otherwise, numbers are not co-prime.
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

bool isCoprime(int a, int b) {
    return gcdEuclidean(a, b) == 1;
}

int main() {
    Header("CO-PRIME CHECK");

    int a1 = 35, b1 = 64;
    int a2 = 21, b2 = 14;

    std::cout << a1 << " and " << b1 << " are "
        << (isCoprime(a1, b1) ? "CO-PRIME" : "NOT CO-PRIME") << std::endl;

    std::cout << a2 << " and " << b2 << " are "
        << (isCoprime(a2, b2) ? "CO-PRIME" : "NOT CO-PRIME") << std::endl;

    Footer();
    return 0;
}
