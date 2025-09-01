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
  std::cout << "DFA That accepts strings ending at b from {a,b}\n";
  std::cout << "==========\n";

  std::string string;
  if (argc > 1) {
    string = argv[1];
  } else {
    std::cout << "Enter a string: ";
    std::cin >> string;
  }

  std::cout << "Input:" << string << std::endl;

  std::set<char> alphabet = {'a', 'b'};

  for (char c : string) {
    if (alphabet.find(c) == alphabet.end()) {
      isInFinalState = false;
      break;
    }
    isInFinalState = (c == 'b');
  }

  verdict();

  footer();
}
