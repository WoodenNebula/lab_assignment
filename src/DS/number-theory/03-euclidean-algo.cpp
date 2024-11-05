#include <iostream>

int gcdEuclidean(int x, int y) {
  // std::cout << "(x, y) = (" << x << ", " << y << ")" << std::endl;
  if (y == 0) {
    return x;
  }

  return gcdEuclidean(y, x % y);
}

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

void gcd(int x, int y) {
  std::cout << "gcd(" << x << ", " << y << ") = " << gcdEuclidean(x, y) << std::endl;
}

void gcdX(int x, int y) {
  int a = 0, b = 0;
  std::cout << "gcdExtended(" << x << ", " << y << ") = " << gcdExEuclidean(x, y, a, b) << " , a = " << a << ", b = " << b << std::endl;
}

int main() {
  int x, y;
  std::cout << "Enter x, y: ";
  std::cin >> x >> y;

  std::cout << "gcd(x, y) by Euclidean algo = " << gcdEuclidean(x, y) << std::endl;

  std::cout << std::endl;

  int a, b;
  std::cout << "gcd(x, y) by Extended Euclidean algo = " << gcdExEuclidean(x, y, a, b) << " , a = " << a << ", b = " << b << std::endl;
  std::cout << std::endl;
}
