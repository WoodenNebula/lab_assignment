#include <iostream>
#include <cmath>

int f(int x) {
  return 2 * x + 3;
}

void printCoords(int x) {
  std::cout << "(" << x << ", " << f(x) << ")" << std::endl;
}

int main() {
  int x = 0, n = 0;

  std::cout << "Enter count of pairs of coords to print (n(x)): ";
  std::cin >> n;
  n = std::min(n, 10);

  int i = n;

  while (i > 0) {
    std::cout << "x(" << n - i + 1 << ") = ";
    std::cin >> x;
    printCoords(x);
    i--;
  }
  std::cout << "\nSurab Parajuli    Section: A    Roll: 34" << std::endl;
}