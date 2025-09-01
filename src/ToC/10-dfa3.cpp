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

  if (isInFinalState) {
    return;
  }

  switch (c) {
  case '0':
    is0Seen = true;
    if (is01Seen) {
      isInFinalState = true;
    }
    break;
  case '1':
    if (is0Seen) {
      is0Seen = false;
      is01Seen = true;
    } else {
      is0Seen = false;
      is01Seen = false;
    }
    break;
  default:
    isInFinalState = false;
    break;
  }
}

int main(int argc, char *argv[]) {
  std::cout << "==========\n";
  std::cout << "DFA That accepts strings having substring 010 from {0,1}*\n";
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
