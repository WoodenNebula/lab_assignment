/* Algorithm (Modular Arithmetic Operations):
1. Start
2. Define modular operations for modulus `m`:
   a. Addition      : (a + b) mod m
   b. Subtraction   : (a - b) mod m
   c. Multiplication: (a * b) mod m
   d. Exponentiation: (a^b) mod m using fast exponentiation
   e. Division      : (a / b) mod m = a * b^{-1} mod m, if inverse exists
3. Use Extended Euclidean Algorithm to find multiplicative inverse of `b` mod m.
4. Display all computed modular results.
5. End
*/

#include <iostream>

#include "src/commons.hpp"

int modNormalize(int x, int m) {
    int v = x % m;
    if (v < 0) v += m;
    return v;
}

int modAdd(int a, int b, int m) {
    return modNormalize(a + b, m);
}

int modSub(int a, int b, int m) {
    return modNormalize(a - b, m);
}

int modMul(int a, int b, int m) {
    long long product = 1LL * modNormalize(a, m) * modNormalize(b, m);
    return (int)(product % m);
}

int modPow(int base, int exp, int m) {
    long long result = 1;
    long long b = modNormalize(base, m);

    while (exp > 0) {
        if (exp & 1) result = (result * b) % m;
        b = (b * b) % m;
        exp >>= 1;
    }

    return (int)result;
}

int extendedGcd(int a, int b, int& x, int& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    int x1, y1;
    int g = extendedGcd(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

bool modInverse(int a, int m, int& inv) {
    int x, y;
    int g = extendedGcd(a, m, x, y);

    if (g != 1) return false;

    inv = modNormalize(x, m);
    return true;
}

int main() {
    Header("MODULAR ARITHMETIC");

    int a = 17;
    int b = 23;
    int m = 11;

    std::cout << "a = " << a << ", b = " << b << ", m = " << m << std::endl;
    std::cout << "(a + b) mod m = " << modAdd(a, b, m) << std::endl;
    std::cout << "(a - b) mod m = " << modSub(a, b, m) << std::endl;
    std::cout << "(a * b) mod m = " << modMul(a, b, m) << std::endl;
    std::cout << "(a ^ b) mod m = " << modPow(a, b, m) << std::endl;

    int invB;
    if (modInverse(b, m, invB))
        std::cout << "(a / b) mod m = " << modMul(a, invB, m)
        << "  (using inverse of b = " << invB << ")" << std::endl;
    else
        std::cout << "(a / b) mod m is undefined (inverse of b does not exist)."
        << std::endl;

    Footer();
    return 0;
}
