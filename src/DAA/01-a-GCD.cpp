#include <iostream>

#include "src/commons.hpp"

int GCD(int x, int y) {
    while (y > 0) {
        int q = x / y;
        int r = x % y;

        x = y;
        y = r;
    }

    return x;
}

int main() {
    Header("GCD");
    int x, y;

    std::cout << "Enter two numbers to find their GCD: ";
    std::cin >> x >> y;

    std::cout << "GCD(" << x << ", " << y << ") : " << GCD(x, y) << std::endl;

    Footer();

    return 0;
}
