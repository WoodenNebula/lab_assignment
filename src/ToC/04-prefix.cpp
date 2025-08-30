#include <iostream>
#include <string>

#include "src/commons.h"

int main(int argc, char *argv[]) {
  std::string parentString = "AbcDe";
  std::cout << "Parent String = " << parentString << "\n";

  int count = 1;
  while (count <= parentString.length()) {
    std::cout << parentString.substr(0, count) << "\n";
    count++;
  }

  footer();
}
