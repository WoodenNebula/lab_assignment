#include <iostream>

int gcdExEuclidean(int x, int y, int& a, int& b) {
  int a2 = 1, a1 = 0;
  int b2 = 0, b1 = 1;

  while (y > 0) {
    int q = x / y;
    int r = x % y;

    a = a2 - q * a1;
    b = b2 - q * b1;

    x = y;
    y = r;

    a2 = a1, a1 = a;

    b2 = b1, b1 = b;
  }

  a = a2;
  b = b2;
  return x;
}

int mulInverse(int a, int m) {
  int x, y;
  const bool isGCD = gcdExEuclidean(a, m, x, y) == 1;

  x = (x + m) % m;
  y = (y + m) % m;

  if (!isGCD) {
    std::cout << "Multiplicative inverse does not exist" << std::endl;
    return -1;
  }

  return x;
}

int addInverse(int a, int m) {
  return (m - a + m) % m;
}

int main() {
  int a, m;
  std::cout << "Enter a, in modulo m: ";
  std::cin >> a >> m;

  int mulInv = mulInverse(a, m);
  if (mulInv != -1) {
    std::cout << "Multiplicative inverse of " << a << " in modulo " << m << " is " << mulInv << std::endl;
  }

  std::cout << "Additive inverse of " << a << " in modulo " << m << " is " << addInverse(a, m) << std::endl;
}