#include "./set-helper.h"

namespace SET
{
  Set Union(const Set& setA, const Set& setB) {
    Set setU(setA);

    for (const auto& x : setB) {
      auto indx = std::find(setA.begin(), setA.end(), x);
      bool isCommonElement = indx != setA.end();
      if (!isCommonElement) {
        setU.push_back(x);
      }
    }

    std::sort(setU.begin(), setU.end());
    return setU;
  }

  Set Intersection(const Set& setA, const Set& setB) {
    Set setN;

    for (const auto& x : setB) {
      auto indx = std::find(setA.begin(), setA.end(), x);
      bool isCommonElement = indx != setA.end();
      if (isCommonElement) {
        setN.push_back(x);
      }
    }

    std::sort(setN.begin(), setN.end());
    return setN;
  }

  Set Difference(const Set& setA, const Set& setB) {
    Set diff(setA);

    for (const auto& x : setB) {
      if (diff.size() <= 0) {
        break;
      }

      auto indx = std::find(diff.begin(), diff.end(), x);
      bool isCommonElement = indx != diff.end();

      if (isCommonElement) {
        diff.erase(indx);
      }
    }

    return diff;
  }
};

int main() {
  using namespace SET;
  Set A = InputSet('A');
  Set B = InputSet('B');

  std::cout << "A u B = " << Union(A, B) << std::endl;
  std::cout << "A n B = " << Intersection(A, B) << std::endl;
  std::cout << "A - B = " << Difference(A, B) << std::endl;
}