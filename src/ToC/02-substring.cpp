#include <iostream>
#include <string>

#include "src/commons.h"

int main(int argc, char *argv[]) {
  std::string parentString = "Hello6789World!";
  std::cout << "Parent String = " << parentString << "\n";

  int pos;
  std::cout << "Enter pos in parent string from which substring begins (pos "
               "character exclusive): ";
  std::cin >> pos;

  int count;
  std::cout << "Enter length of substring: ";
  std::cin >> count;

  std::cout << "Substring is: " << parentString.substr(pos, count) << std::endl;
  footer();
  return 0;
}
