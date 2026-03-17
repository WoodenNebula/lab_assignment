/* Algorithm (Lucas-Lehmer Test for Mersenne Numbers):
1. Start
2. For exponent p (prime), define Mersenne number:
      M = 2^p - 1
3. Lucas-Lehmer sequence:
   a. Set s = 4
   b. Repeat (p - 2) times:
        s = (s*s - 2) mod M
4. If final s == 0, M is prime; otherwise M is composite.
5. End

Note: Lucas-Lehmer is specifically for Mersenne numbers (2^p - 1).
*/

#include <cstdint>
#include <iostream>
#include <vector>

#include "src/commons.hpp"

bool isPrimeSmall(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

bool lucasLehmer(int p) {
    if (p == 2) return true;

    uint64_t M = (1ULL << p) - 1;
    uint64_t s = 4;

    for (int i = 0; i < p - 2; i++)
        s = (s * s - 2) % M;

    return s == 0;
}

int main() {
    Header("LUCAS-LEHMER TEST");

    std::vector<int> exponents = { 7, 11 };

    for (int p : exponents) {
        if (!isPrimeSmall(p)) {
            std::cout << "p = " << p << " is not prime; test skipped." << std::endl;
            continue;
        }

        uint64_t M = (1ULL << p) - 1;
        bool prime = lucasLehmer(p);

        std::cout << "M_" << p << " = " << M << " is "
            << (prime ? "PRIME" : "COMPOSITE") << std::endl;
    }

    Footer();
    return 0;
}
