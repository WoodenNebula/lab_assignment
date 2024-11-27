#include <cmath>

long long int fact(long long int n) {
  if (n <= 0) {
    return 1;
  }

  return n * fact(n - 1);
}

long long perm(long long  n, long long r) {
  return fact(n) / fact(n - r);
}
long long comb(long long  n, long long r) {
  return fact(n) / (fact(r) * fact(n - r));
}