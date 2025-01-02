#include "../set-theory/set-helper.h"

namespace FUZZY_SET
{
  FuzzySet Union(const FuzzySet& setA, const FuzzySet& setB) {
    FuzzySet setU(setA);

    for (size_t i = 0; i < setA.size(); i++) {
      setU[i] = std::max(setA[i], setB[i]);
    }

    return setU;
  }
  FuzzySet Intersection(const FuzzySet& setA, const FuzzySet& setB) {
    FuzzySet setN(setA);

    for (size_t i = 0; i < setA.size(); i++) {
      setN[i] = std::min(setA[i], setB[i]);
    }

    return setN;
  }
  FuzzySet Complement(const FuzzySet& set) {
    FuzzySet setC(set);

    for (size_t i = 0; i < setC.size(); i++) {
      setC[i] = 1 - setC[i];
    }

    return setC;
  }
}

int main() {
  using namespace FUZZY_SET;

  FuzzySet A = InputSet(0, 'A');
  FuzzySet B = InputSet(A.size(), 'B');

  std::cout << "\nFuzzy Set A = " << A << std::endl;
  std::cout << "Fuzzy Set B = " << B << std::endl;

  std::cout << "\nA u B = " << Union(A, B) << std::endl;
  std::cout << "A n B = " << Intersection(A, B) << std::endl;
  std::cout << "A\' = " << Complement(A) << std::endl;
  std::cout << "\nSurab Parajuli    Section: A    Roll: 34" << std::endl;
}