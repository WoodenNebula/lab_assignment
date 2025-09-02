#include "src/commons.h"
#include <iostream>
#include <stack>
#include <string_view>
#include <vector>

bool isInFinalState = false;

void verdict(const std::string_view &s) {
  std::cout << "Input: " << (s.empty() ? "ε" : s) << " -> "
            << (isInFinalState ? std::string(GREEN) + "Accepted"
                               : std::string(RED) + "Rejected");
  std::cout << RESET << std::endl;
}

bool simulatePDA(const std::string_view &input) {
  // dont accept empty string
  if (input.size() == 0) {
    return false;
  }

  std::stack<char> st;
  st.push('Z'); // bottom marker

  int i = 0;
  for (char c : input) {
    if (c == '0') {
      // push marker for 0
      st.push('X');
    } else if (c == '1') {
      if (st.empty() || st.top() != 'X') {
        return false; // no matching 0 for this 1
      }
      st.pop(); // match one 0 with this 1
    } else {
      // invalid symbol
      return false;
    }
  }

  // accept if only bottom marker remains
  return (st.size() == 1 && st.top() == 'Z');
}

int main() {
  std::vector<std::string_view> testCases = {
      "0011",   // valid
      "000111", // valid
      "01",     // valid
      "10",     // invalid
      "001",    // invalid
      "011",    // invalid
      ""        // valid
  };

  for (auto &s : testCases) {
    isInFinalState = simulatePDA(s);
    verdict(s);
  }

  return 0;
}
