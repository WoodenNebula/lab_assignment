#include <iostream>

int power(int base, int exponent) {
  if (exponent == 0) {
    return 1;
  }

  return base * power(base, exponent - 1);
}

int main() {
  int base, exponent;
  std::cout << "Enter base, exponent: ";
  std::cin >> base >> exponent;

  std::cout << base << "^" << exponent << " = " << power(base, exponent) << std::endl;
}