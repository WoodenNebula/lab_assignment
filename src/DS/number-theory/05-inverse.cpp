#include <iostream>
#include "./algo.h"

int addInverse(int a, int m) {
  return (m - a + m) % m;
}

int main() {
  int a, m;
  std::cout << "Enter a, in modulo m: ";
  std::cin >> a >> m;

  int mulInv = Algo::mulInverse(a, m);
  if (mulInv == -1) {
    std::cout << "Multiplicative inverse does not exist" << std::endl;
  }
  std::cout << "Multiplicative inverse of " << a << " in modulo " << m << " is " << mulInv << std::endl;

  std::cout << "Additive inverse of " << a << " in modulo " << m << " is " << addInverse(a, m) << std::endl;
}