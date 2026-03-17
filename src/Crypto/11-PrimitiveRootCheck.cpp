/* Algorithm (Primitive Root Check):
1. Start
2. For given integers g and n:
   a. If gcd(g, n) != 1, g cannot be a primitive root of n.
3. Compute phi(n).
4. Find prime factors of phi(n).
5. For each prime factor q of phi(n):
   a. Compute g^(phi(n)/q) mod n.
   b. If result == 1 for any q, g is not a primitive root.
6. If all checks pass, g is a primitive root of n.
7. End
*/

#include <iostream>
#include <vector>

#include "src/commons.hpp"

long long gcdLL(long long a, long long b) {
    while (b != 0) {
        long long r = a % b;
        a = b;
        b = r;
    }
    return a;
}

long long modPow(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;

    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    return result;
}

int eulerTotient(int n) {
    int result = n;

    for (int p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0)
                n /= p;
            result -= result / p;
        }
    }

    if (n > 1)
        result -= result / n;

    return result;
}

std::vector<int> primeFactors(int n) {
    std::vector<int> factors;

    for (int p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            factors.push_back(p);
            while (n % p == 0)
                n /= p;
        }
    }

    if (n > 1)
        factors.push_back(n);

    return factors;
}

bool isPrimitiveRoot(int g, int n) {
    if (gcdLL(g, n) != 1) return false;

    int phi = eulerTotient(n);
    std::vector<int> factors = primeFactors(phi);

    for (int q : factors)
        if (modPow(g, phi / q, n) == 1)
            return false;

    return true;
}

int main() {
    Header("PRIMITIVE ROOT CHECK");

    int g1 = 3, n = 17;
    int g2 = 4;

    std::cout << g1 << (isPrimitiveRoot(g1, n) ? " is " : " is not ")
        << "a primitive root of " << n << std::endl;

    std::cout << g2 << (isPrimitiveRoot(g2, n) ? " is " : " is not ")
        << "a primitive root of " << n << std::endl;

    Footer();
    return 0;
}
