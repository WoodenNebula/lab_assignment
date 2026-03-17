/* Algorithm (Euler Totient Function phi(n)):
1. Start
2. Set result = n.
3. For each prime factor p of n:
   a. If p divides n, remove all factors of p from n.
   b. Update result = result - result / p.
4. If remaining n > 1, it is a prime factor > sqrt(original n):
   result = result - result / n.
5. Result is phi(original n), the count of numbers in [1, n]
   that are coprime with n.
6. End
*/

#include <iostream>

#include "src/commons.hpp"

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

int main() {
    Header("EULER TOTIENT FUNCTION");

    int n = 36;
    std::cout << "phi(" << n << ") = " << eulerTotient(n) << std::endl;

    std::cout << "\nphi(n) for n = 1 to 10:\n";
    for (int i = 1; i <= 10; i++)
        std::cout << "phi(" << i << ") = " << eulerTotient(i) << std::endl;

    Footer();
    return 0;
}
