#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
std::ostream& operator <<(std::ostream& out, const std::vector<T>& set) {
  out << "{";
  if (set.size() > 0) {
    for (size_t i = 0; i < set.size() - 1; i++) {
      out << set[i] << ", ";
    }
    out << set[set.size() - 1] << "}";
  }
  else {
    out << "NullSet}";
  }

  return out;
}

namespace SET
{
  using Set = std::vector<int>;

  Set InputSet(char set = 'A') {
    Set _set;
    std::cout << "n(" << set << ") = ";
    size_t count = 0;
    std::cin >> count;

    int x = 0;
    for (size_t i = 0; i < count; i++) {
      bool isDuplicate = false;
      do {
        std::cin >> x;
        isDuplicate = std::find(_set.begin(), _set.end(), x) != _set.end();
        if (isDuplicate) {
          std::cerr << "Element must be unique:";
        }
      } while (isDuplicate);

      _set.push_back(x);
    }
    std::sort(_set.begin(), _set.end());
    return _set;
  }
};