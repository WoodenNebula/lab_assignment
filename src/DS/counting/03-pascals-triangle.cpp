#include <iostream>
#include "./Math.h"

void printTriangle(int row) {
  for (int n = 0; n < row; n++) {
    for (int k = 0; k <= n; k++) {
      std::cout << comb(n, k) << ' ';
    }
    std::cout << std::endl;
  }
}

int main() {
  printTriangle(4);
  std::cout << "\nSurab Parajuli    Section: A    Roll: 34" << std::endl;
}