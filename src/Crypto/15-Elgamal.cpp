/* Algorithm (ElGamal Cryptosystem):
1. Start
2. Key generation:
   a. Choose large prime p and generator g.
   b. Choose private key x (1 < x < p-1).
   c. Compute public key y = g^x mod p.
3. Encryption of message m:
   a. Choose random session key k with gcd(k, p-1)=1.
   b. c1 = g^k mod p
   c. c2 = (m * y^k) mod p
   d. Cipher text = (c1, c2)
4. Decryption:
   a. s = c1^x mod p
   b. Compute s^{-1} mod p
   c. m = c2 * s^{-1} mod p
5. End
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
    Header("ELGAMAL CRYPTOSYSTEM");

    long long p = 467;
    long long g = 2;

    long long x = 127;                 // private key
    long long y = modPow(g, x, p);     // public key component

    long long m = 123;                 // message
    long long k = 53;                  // session key

    long long c1 = modPow(g, k, p);
    long long s = modPow(y, k, p);
    long long c2 = (m * s) % p;

    long long sDec = modPow(c1, x, p);
    long long sInv = modPow(sDec, p - 2, p);   // Fermat inverse since p is prime
    long long mDec = (c2 * sInv) % p;

    std::cout << "Public Key : (p=" << p << ", g=" << g << ", y=" << y << ")"
        << std::endl;
    std::cout << "Private Key: x=" << x << std::endl;

    std::cout << "\nMessage m : " << m << std::endl;
    std::cout << "Cipher (c1, c2) = (" << c1 << ", " << c2 << ")" << std::endl;
    std::cout << "Decrypted message: " << mDec << std::endl;

    Footer();
    return 0;
}
