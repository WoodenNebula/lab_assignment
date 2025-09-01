#include <iostream>
#include <set>
#include <string>

#include "src/commons.h"

bool isInFinalState = false;

void verdict() {
  std::cout << (isInFinalState ? GREEN : RED);
  std::cout << (isInFinalState ? "Accepted" : "Rejected");
  std::cout << RESET << std::endl;
}

int main(int argc, char *argv[]) {
  std::cout << "==========\n";
  std::cout << "NFA That accepts strings starting with 01 from {0,1}*\n";
  std::cout << "==========\n";

  std::string string;
  if (argc > 1) {
    string = argv[1];
  } else {
    std::cout << "Enter a string: ";
    std::cin >> string;
  }

  std::cout << "Input:" << string << std::endl;

  std::set<char> alphabet = {'0', '1'};

  isInFinalState = string.substr(0, 2).compare("01") == 0;
  verdict();

  footer();
}
