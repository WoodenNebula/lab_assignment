#include <iostream>
#include <string>

#include "src/commons.h"

int main(int argc, char *argv[]) {
  std::string parentString = "AbcDe";
  std::cout << "Parent String = " << parentString << "\n";

  int startPos = 0;
  while (startPos < parentString.length()) {
    int count = 1;
    while (count <= parentString.length() - startPos) {
      std::cout << parentString.substr(startPos, count) << "\n";
      count++;
    }
    startPos++;
  }

  footer();
}
