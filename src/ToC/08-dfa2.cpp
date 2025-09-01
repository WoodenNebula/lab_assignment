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

void transition(char c) {
  static bool is0Seen = false;
  static bool is01Seen = false;

  switch (c) {
  case '0':
    is0Seen = true;
    is01Seen = false;
    isInFinalState = false;
    break;
  case '1':
    if (is0Seen) {
      is0Seen = false;
      is01Seen = true;
      isInFinalState = true;
      break;
    } else {
      is0Seen = false;
      is01Seen = false;
      isInFinalState = false;
      break;
    }
  default:
    isInFinalState = false;
    break;
  }
}

int main(int argc, char *argv[]) {
  std::cout << "==========\n";
  std::cout << "DFA That accepts strings ending at 01 from {0,1}*\n";
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

  // driver code
  for (char c : string) {
    if (alphabet.find(c) == alphabet.end()) {
      isInFinalState = false;
      break;
      std::cerr << RED << "Invalid Char" << RESET << std::endl;
    }
    transition(c);
  }

  verdict();

  footer();
}
