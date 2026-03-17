/* Algorithm (Multiplicative Inverse using Extended Euclidean Algorithm):
1. Start
2. Apply extended Euclidean algorithm to a and m:
      ax + my = gcd(a, m)
3. If gcd(a, m) != 1, inverse does not exist.
4. If gcd(a, m) == 1, x is the modular inverse of a modulo m.
5. Normalize x to [0, m-1] using x mod m.
6. End
*/

#include <iostream>

#include "src/commons.hpp"

long long extendedGcd(long long a, long long b, long long& x, long long& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    long long x1, y1;
    long long g = extendedGcd(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

bool multiplicativeInverse(long long a, long long m, long long& inv) {
    long long x, y;
    long long g = extendedGcd(a, m, x, y);

    if (g != 1) return false;

    inv = x % m;
    if (inv < 0) inv += m;
    return true;
}

int main() {
    Header("MULTIPLICATIVE INVERSE");

    long long a = 17;
    long long m = 3120;
    long long inv;

    if (multiplicativeInverse(a, m, inv)) {
        std::cout << "Inverse of " << a << " modulo " << m << " is " << inv
            << std::endl;
        std::cout << "Verification: (" << a << " * " << inv << ") mod " << m
            << " = " << (a * inv) % m << std::endl;
    }
    else {
        std::cout << "Inverse does not exist." << std::endl;
    }

    Footer();
    return 0;
}
