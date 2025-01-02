#include <iostream>

int f(int x) {
  return 2 * x + 3;
}

int g(int x) {
  return 3 * x + 2;
}

int composite(int f(int), int g(int), int x) {
  return f(g(x));
}

int main() {
  int x = 0;
  std::cout << "Enter x: ";
  std::cin >> x;

  std::cout << "fog = " << composite(f, g, x) << std::endl;
  std::cout << "gof = " << composite(g, f, x) << std::endl;
  std::cout << "\nSurab Parajuli    Section: A    Roll: 34" << std::endl;
}