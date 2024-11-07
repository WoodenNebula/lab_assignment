#include <iostream>

int sum(int n) {
  if (n <= 0) {
    return 0;
  }

  return n % 10 + sum(n / 10);
}

int main() {
  int n;
  std::cout << "Enter n: ";
  std::cin >> n;

  std::cout << "Sum of digits of n = " << sum(n) << std::endl;
}