/* Algorithm (RSA Key Generation and Demo):
1. Start
2. Choose two primes p and q.
3. Compute:
   a. n   = p * q
   b. phi = (p - 1) * (q - 1)
4. Choose e such that 1 < e < phi and gcd(e, phi) = 1.
5. Compute private exponent d as modular inverse of e mod phi:
      d * e mod phi = 1
6. Public key  = (e, n)
   Private key = (d, n)
7. Encryption:
      C = M^e mod n
8. Decryption:
      M = C^d mod n
9. End
*/

#include <iostream>

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

long long modInverse(long long a, long long m) {
    long long x, y;
    extendedGcd(a, m, x, y);

    x %= m;
    if (x < 0) x += m;
    return x;
}

int main() {
    Header("RSA KEY GENERATION");

    long long p = 61;
    long long q = 53;

    long long n = p * q;
    long long phi = (p - 1) * (q - 1);

    long long e = 17;
    if (gcdLL(e, phi) != 1) {
        for (e = 2; e < phi; e++)
            if (gcdLL(e, phi) == 1)
                break;
    }

    long long d = modInverse(e, phi);

    std::cout << "p = " << p << ", q = " << q << std::endl;
    std::cout << "n = p*q = " << n << std::endl;
    std::cout << "phi(n) = " << phi << std::endl;
    std::cout << "Public Key  (e, n) = (" << e << ", " << n << ")" << std::endl;
    std::cout << "Private Key (d, n) = (" << d << ", " << n << ")" << std::endl;

    long long message = 65;
    long long cipher = modPow(message, e, n);
    long long plain = modPow(cipher, d, n);

    std::cout << "\nMessage   : " << message << std::endl;
    std::cout << "Encrypted : " << cipher << std::endl;
    std::cout << "Decrypted : " << plain << std::endl;

    Footer();
    return 0;
}
