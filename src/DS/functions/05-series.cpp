#include <iostream>
#include <string>
#include <sstream>

// series = 1/2 + 2/5 + 3/10 + 4/17 + ...

std::string getNthTerm(int n) {
  int numeratorStart = 1;

  int n_top = n * numeratorStart;
  int n_bottom = n_top * n_top + 1;

  std::stringstream ss;

  ss << n_top << "/" << n_bottom;

  return ss.str();
}

int main() {
  const int N_TERM = 10;

  for (int i = 1; i < N_TERM; i++) {
    std::cout << getNthTerm(i) << ", ";
  }

  std::cout << getNthTerm(N_TERM) << std::endl;
  std::cout << "\nSurab Parajuli    Section: A    Roll: 34" << std::endl;
}