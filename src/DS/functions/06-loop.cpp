#include <iostream>
// program to find sum of product of i, j : 1<=i<=5, 1<=j<=4
int main() {
  const int iMax = 5;
  const int jMax = 4;

  int sum = 0;
  for (int i = 1; i <= iMax; i++) {
    for (int j = 1;j <= jMax;j++) {
      sum += i * j;
    }
  }

  std::cout << sum << std::endl;
}