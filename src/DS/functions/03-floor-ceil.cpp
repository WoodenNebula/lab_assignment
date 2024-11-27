#include <iostream>

int Floor(double x) {
  int _x = (int)x;
  std::cout << "Floored value of " << x << " = " << _x << std::endl;
  return _x;
}

int Ceil(double x) {
  int _x = (int)x + 1;
  std::cout << "Ceiled value of " << x << " = " << _x << std::endl;
  return _x;
}

int main() {
  double x;
  std::cout << "Enter a real number: ";
  std::cin >> x;

  Floor(x);
  Ceil(x);
}