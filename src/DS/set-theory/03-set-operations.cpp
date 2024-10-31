#include "./set-helper.h"
using Set = std::vector<int>;

Set setUnion(const Set& setA, const Set& setB) {
  std::vector<int> setU(setA);

  for (const int& x : setB) {
    if (std::find(setU.begin(), setU.end(), x) == setU.end()) {
      setU.push_back(x);
    }
  }

  return setU;
}

int main() {
  Set A = SET::inputSet('A');
  Set B = SET::inputSet('B');

  Set U = setUnion(A, B);

  std::cout << "A u B = " << U << std::endl;
}