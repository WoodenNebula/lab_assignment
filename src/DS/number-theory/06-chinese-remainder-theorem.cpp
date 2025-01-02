#include <iostream>
#include <vector>
#include "./algo.h"

int findX(const std::vector<int>& mod, const std::vector<int>& rem) {
  int x = 0;
  int product = 1;

  for (int i = 0; i < mod.size(); i++) {
    product *= mod[i];
  }

  for (int i = 0; i < mod.size(); i++) {
    int partialProduct = product / mod[i];
    x += rem[i] * partialProduct * Algo::mulInverse(partialProduct, mod[i]);
  }

  return x % product;
}


int main() {
  std::vector<int> modulos = { 3, 4, 5 };
  std::vector<int> remainders = { 2, 3, 1 };

  std::cout << "x = " << findX(modulos, remainders) << std::endl;
  std::cout << "\nSurab Parajuli    Section: A    Roll: 34" << std::endl;
}