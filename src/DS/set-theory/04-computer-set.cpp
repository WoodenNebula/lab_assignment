#include "./set-helper.h"
#include <string>

size_t findInUniversalSet(const std::vector<int>& setU, int valueToFind) {
  for (size_t i = 0; i < setU.size(); i++) {
    if (setU.at(i) == valueToFind) {
      return i;
    }
  }
  return -1;
}

int main() {
  using namespace SET;
  Set setU = InputSet('U');
  Set setA = InputSet('A');

  std::string bitString(setU.size(), '0');

  for (size_t i = 0; i < setA.size(); i++) {
    size_t index = findInUniversalSet(setU, setA.at(i));
    bool isMember = index != -1;
    if (isMember) {
      bitString[index] = '1';
    }
  }

  std::cout << "Bit string representation is: " << bitString << std::endl;
}