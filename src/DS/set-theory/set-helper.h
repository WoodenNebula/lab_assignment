#include <iostream>
#include <vector>
#include <algorithm>

namespace SET
{
  std::vector<int> inputSet(char set = 'A') {
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
    std::sort(_set.begin(), _set.end());
    return _set;
  }


};
template <typename T>
std::ostream& operator <<(std::ostream& out, const std::vector<T>& set) {
  out << "{";
  for (size_t i = 0; i < set.size() - 1; i++) {
    out << set[i] << ", ";
  }
  out << set[set.size() - 1] << "}";

  return out;
}