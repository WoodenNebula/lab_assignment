#include <iostream>

int gcdEuclidean(int x, int y) {
  // std::cout << "(x, y) = (" << x << ", " << y << ")" << std::endl;
  if (y == 0) {
    return x;
  }

  return gcdEuclidean(y, x % y);
}

bool isGcd(int x, int y) {
  return gcdEuclidean(x, y) == 1;
}



int main() {
  int x, y;
  std::cout << "Enter x, y: ";
  std::cin >> x >> y;

  std::cout << (isGcd(x, y) ? "Co-Prime" : "Not Co-Prime") << std::endl;
}
