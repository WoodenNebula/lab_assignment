#include "./set-helper.h"


int main() {
  std::vector<int> setA = SET::inputSet('A');
  std::vector<int> setB = SET::inputSet('B');

  // std::cout << setA << ", " << setB << std::endl;

  std::vector<std::vector<int>> cartesianProduct;

  for (const int& x : setA) {
    for (const int& y : setB) {
      cartesianProduct.push_back({ x, y });
    }
  }

  std::cout << "\nCartesian Product:\n" << cartesianProduct << std::endl;
}