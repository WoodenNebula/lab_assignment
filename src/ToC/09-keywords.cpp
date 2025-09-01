#include <iostream>
#include <set>
#include <string>
#include <string_view>

#include "src/commons.h"

bool isInFinalState = false;

void verdict() {
  std::cout << (isInFinalState ? GREEN : RED);
  std::cout << (isInFinalState ? "Accepted" : "Rejected");
  std::cout << RESET << std::endl;
}

enum class E_Alphabet { NONE, DIGIT, LETTER, UNDERSCORE };

E_Alphabet ClassifySymbol(char c) {
  if (c == '_') {
    return E_Alphabet::UNDERSCORE;
  } else if (c >= '0' && c <= '9') {
    return E_Alphabet::DIGIT;
  } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
    return E_Alphabet::LETTER;
  } else {
    return E_Alphabet::NONE;
  }
}

bool isValidIdentifier(const std::string_view &string) {
  if (ClassifySymbol(string[0]) == E_Alphabet::DIGIT) {
    return false;
  }

  for (char c : string) {
    E_Alphabet a = ClassifySymbol(c);
    if (a == E_Alphabet::NONE) {
      return false;
    }
  }

  return true;
}

int main(int argc, char *argv[]) {
  std::string string;
  if (argc > 1) {
    string = argv[1];
  } else {
    std::cout << "Enter a string: ";
    std::cin >> string;
  }

  std::cout << "Input:" << string << std::endl;

  std::set<std::string_view> keywords = {
      "const",    "int",      "float",  "double", "long",    "char",
      "void",     "unsigned", "short",  "signed", "size_t",  "struct",
      "enum",     "union",    "switch", "case",   "default", "if",
      "else",     "while",    "do",     "for",    "break",   "continue",
      "return",   "goto",     "static", "extern", "auto",    "register",
      "volatile", "typedef"};

  isInFinalState = keywords.find(string) != keywords.end();
  if (isInFinalState) {
    verdict();
    std::cout << "--By Keyword" << std::endl;
  } else {
    isInFinalState = isValidIdentifier(string);
    if (isInFinalState) {
      verdict();
      std::cout << "--By Valid Identifier" << std::endl;
    } else {
      verdict();
    }
  }

  footer();
}
