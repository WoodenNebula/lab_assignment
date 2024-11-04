#include <iostream>
#include <cmath>

struct Pair
{
  int k;
  int m;
};


Pair findPair(int q, int r, int i) {
  int remainder = q % 2;
  int quotient = q / 2;
  int iteration = i + 1;

  // std::cout << "(q, r, itr) = (" << q << ", " << r << ", " << iteration << ")" << std::endl;

  if (remainder != 0) {
    return { i, q };
  }

  return findPair(quotient, remainder, iteration);
}


bool  isPrime(int n) {
  const auto [k, m] = findPair(n - 1, 0, 0);
  // std::cout << "(k, m) = (" << k << ", " << m << ")" << std::endl;

  int x = 0;
  int a = 2;

  x = ((int)std::pow(a, m)) % n;

  if (x == 1 || x == n - 1)
    return true;

  for (int i = 0; i < k - 1; i++) {
    x = (x * x) % n;

    // std::cout << "x = " << x << std::endl;

    if (x == 1) {
      return false;
    }
    else if (x == n - 1) {
      return true;
    }
  }

  return false;
}

int main() {
  int n = 0;
  std::cout << "Enter a number(n): ";
  std::cin >> n;

  std::cout << (isPrime(n) ? "Prime" : "Composite") << std::endl;
}