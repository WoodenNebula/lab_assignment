#include <iostream>
#include <ostream>

#include "src/commons.hpp"

// Find nth fibonnacci
int main() {
    Header("n-th Fibonnacci");
    int n = 1;
    std::cout << "Enter value of n: ";
    std::cin >> n;

    int last = 1, nth = 1;
    for (int i = 2; i < n; i++) {
        int tmp = nth;
        nth += last;
        last = tmp;
    }

    std::cout << n << "th fibonnacci number is " << nth;
    Footer();
}
