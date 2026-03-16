#include <iostream>
#include <vector>

#include "src/commons.hpp"

int main() {
    Header("SEQUENTIAL SEARCH");
    std::vector<int> haystack = {1, 2, 6, 9, 10, 34, 4, 0, 19};
    std::cout << "[ ";
    for (int x : haystack) {
        std::cout << x << ", ";
    }
    std::cout << "]\n";

    int needle = 0;
    std::cout << "Enter a number to find in the array: ";
    std::cin >> needle;

    bool was_found = false;
    size_t idx;

    for (idx = 0; idx < haystack.size(); idx++) {
        if (haystack[idx] == needle) {
            was_found = true;
            break;
        }
    }
    if (was_found)
        std::cout << "Found " << needle << " at index " << idx << std::endl;
    else
        std::cout << needle << " not found";

    Footer();
}
