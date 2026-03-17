/* Algorithm (Diffie-Hellman Key Exchange):
1. Start
2. Publicly agree on prime p and generator g.
3. Alice chooses private key a.
   Bob chooses private key b.
4. Alice computes A = g^a mod p and sends A to Bob.
5. Bob computes B = g^b mod p and sends B to Alice.
6. Alice computes shared key K1 = B^a mod p.
7. Bob computes shared key K2 = A^b mod p.
8. Since K1 == K2, both obtain the same secret key.
9. End
*/

#include <iostream>

#include "src/commons.hpp"

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

int main() {
    Header("DIFFIE-HELLMAN KEY EXCHANGE");

    long long p = 23;   // public prime
    long long g = 5;    // public primitive root

    long long a = 6;    // Alice private key
    long long b = 15;   // Bob private key

    long long A = modPow(g, a, p);
    long long B = modPow(g, b, p);

    long long aliceShared = modPow(B, a, p);
    long long bobShared = modPow(A, b, p);

    std::cout << "Public values: p = " << p << ", g = " << g << std::endl;
    std::cout << "Alice sends A = " << A << std::endl;
    std::cout << "Bob sends B   = " << B << std::endl;
    std::cout << "Alice shared key = " << aliceShared << std::endl;
    std::cout << "Bob shared key   = " << bobShared << std::endl;

    Footer();
    return 0;
}
