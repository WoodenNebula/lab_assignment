#include <iostream>

int mod(int a, int b) {
  return a % b;
}

int main() {
  int a, b;

  std::cout << "Enter a: ";
  std::cin >> a;

  std::cout << "Enter b: ";
  std::cin >> b;

  std::cout << a << " MOD " << b << " = " << mod(a, b) << std::endl;
  std::cout << "\nSurab Parajuli    Section: A    Roll: 34" << std::endl;
}