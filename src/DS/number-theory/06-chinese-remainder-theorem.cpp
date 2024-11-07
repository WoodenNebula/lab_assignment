#include <iostream>
#include <vector>

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

int findX(const std::vector<int>& mod, const std::vector<int>& rem) {
  int x = 0;
  int product = 1;

  for (int i = 0; i < mod.size(); i++) {
    product *= mod[i];
  }

  for (int i = 0; i < mod.size(); i++) {
    int partialProduct = product / mod[i];
    x += rem[i] * partialProduct * mulInverse(partialProduct, mod[i]);
  }

  return x % product;
}


int main() {
  std::vector<int> modulos = { 3, 4, 5 };
  std::vector<int> remainders = { 2, 3, 1 };

  std::cout << "x = " << findX(modulos, remainders) << std::endl;
}