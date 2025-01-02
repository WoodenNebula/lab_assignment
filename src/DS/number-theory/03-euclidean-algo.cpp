#include <iostream>
#include "./algo.h"

using namespace Algo;

int main() {
  int x, y;
  std::cout << "Enter x, y: ";
  std::cin >> x >> y;

  std::cout << "gcd(x, y) by Euclidean algo = " << gcdEuclidean(x, y) << std::endl;

  std::cout << std::endl;

  int a, b;
  std::cout << "gcd(x, y) by Extended Euclidean algo = " << gcdExEuclidean(x, y, a, b) << " , a = " << a << ", b = " << b << std::endl;
  std::cout << std::endl;
  std::cout << "\nSurab Parajuli    Section: A    Roll: 34" << std::endl;
}
