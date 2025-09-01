#include <iostream>
#include <string>

#include "src/commons.h"

void LeftMostSub(const std::string_view &string) {
  std::cout << "--------\n";
  int length;
  std::cout << "Enter length of left most substring: ";
  std::cin >> length;

  std::cout << "LeftMost substring of length 'n' is: "
            << string.substr(0, length) << "\n";
}

void RightMostSub(const std::string_view &string) {
  std::cout << "--------\n";
  int length;
  std::cout << "Enter length of right most substring: ";
  std::cin >> length;

  if (length >= 0) {
    int pos = string.length() - length;
    std::cout << "count=" << length << ", pos=" << pos << std::endl;

    std::cout << "LeftMost substring of length 'n' is: "
              << string.substr(pos, length) << "\n";

  } else {
    std::cerr << "[Err] - {Invalid length}" << std::endl;
  }
}

void PositionalSubstring(const std::string_view &string) {
  std::cout << "--------\n";
  int startPos, endPos;
  std::cout << "For Positional substring, enter:\n\tstartPos(n) = ";
  std::cin >> startPos;
  std::cout << "\tendPos(m) = ";
  std::cin >> endPos;

  if ((startPos >= 0 && endPos >= 0 && startPos <= endPos)) {
    std::cout << "Substring from [n,m]=[" << startPos << ", " << endPos
              << "] is: " << string.substr(startPos, endPos - startPos + 1)
              << "\n";
  } else {
    std::cerr << "[Err] - {Invalid start or end position}" << std::endl;
  }
}

int main(int argc, char *argv[]) {
  std::cout << "Enter a String = ";
  std::string parentString;
  std ::cin >> parentString;

  LeftMostSub(parentString);
  RightMostSub(parentString);
  PositionalSubstring(parentString);

  footer();
  return 0;
}
