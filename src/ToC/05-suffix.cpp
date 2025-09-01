#include <algorithm>
#include <iostream>
#include <string>

#include "src/commons.h"

int main(int argc, char *argv[]) {
  std::string parentString = "AbcDe";
  std::cout << "Parent String = " << parentString << "\n";

  int count = 1;
  for (int pos = parentString.length() - 1; pos >= 0; pos--) {
    std::cout << "count=" << count << ", pos=" << pos << std::endl;
    std::cout << parentString.substr(pos, count) << "\n";
    count++;
  }

  footer();
}
