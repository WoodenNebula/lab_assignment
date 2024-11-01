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

namespace FUZZY_SET
{
  using FuzzySet = std::vector<float>;

  FuzzySet InputSet(size_t count, char set = 'A') {
    std::cout << "For membership set" << set << ":\n";
    std::cout << "n(" << set << ") = ";

    if (count == 0) {
      std::cin >> count;
    }
    else {
      std::cout << count << std::endl;
    }

    FuzzySet _set(count, 0);

    double x = 0;
    for (size_t i = 0; i < count; i++) {
      std::cin >> x;

      _set[i] = x;
    }
    return _set;
  }
}

// namespace FUZZY_SET
// {
//   struct Element {
//     int a;
//     float u;

//     Element() {}

//     Element(int a, float u) :a(a), u(u) {}

//     friend std::ostream& operator<<(std::ostream& out, const Element& e) {
//       out << "(" << e.a << ", " << e.u << ")";
//       return out;
//     }

//     friend std::istream& operator>>(std::istream& in, Element& e) {
//       std::cout << "a = ";
//       in >> e.a;
//       std::cout << "u = ";
//       in >> e.u;
//       std::cout << std::endl;
//       return in;
//     }


//     friend bool operator<(const Element& a, const Element& b) {
//       if (a.a == b.a) {
//         return a.u < b.u;
//       }
//       else {
//         return (a.a < b.a);
//       }
//     }

//     friend bool operator==(Element a, Element b) {
//       return (a.a == b.a) && (a.u == b.u);
//     }
//   };

//   using FuzzySet = std::vector<Element>;

//   FuzzySet InputSet(char set = 'A') {
//     FuzzySet _set;
//     std::cout << "n(" << set << ") = ";
//     size_t count = 0;
//     std::cin >> count;

//     Element e;

//     for (size_t i = 0; i < count; i++) {
//       bool isDuplicate = false;

//       do {
//         std::cin >> e;
//         isDuplicate = std::find(_set.begin(), _set.end(), e) != _set.end();

//         if (isDuplicate) {
//           std::cerr << "Element must be unique:\n";
//         }
//       } while (isDuplicate);

//       _set.push_back(e);
//     }
//     std::sort(_set.begin(), _set.end());
//     return _set;
//   }
// };