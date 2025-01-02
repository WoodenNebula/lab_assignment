#include <iostream>
#include "./algo.h"

bool isCoPrime(int x, int y) {
  return Algo::gcdEuclidean(x, y) == 1;
}

int main() {
  int x, y;
  std::cout << "Enter x, y: ";
  std::cin >> x >> y;

  std::cout << (isCoPrime(x, y) ? "Co-Prime" : "Not Co-Prime") << std::endl;
  std::cout << "\nSurab Parajuli    Section: A    Roll: 34" << std::endl;
}