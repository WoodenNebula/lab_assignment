#include "./set-helper.h"

std::vector<int> InputSet(char set = 'A') {
  std::vector<int> _set;
  std::cout << "n(" << set << ") = ";
  size_t count = 0;
  std::cin >> count;

  int x = 0;
  for (size_t i = 0; i < count; i++) {
    bool match = false;
    do {
      std::cin >> x;
      match = std::find(_set.begin(), _set.end(), x) != _set.end();
      if (match) {
        std::cerr << "Element must be unique:";
      }
    } while (match);

    _set.push_back(x);
  }
  return _set;
}

int main() {
  std::vector<int> set = InputSet();
  std::cout << set;
}