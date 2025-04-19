#include "Utility.h"
#include <iostream>
#include <limits>

#include "Maths.h"

namespace CG::Utility
{
CG::Maths::Vec3f getRandomColor() {
    srand(static_cast<unsigned int>(time(0)));
    return { static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX };
};

void flush() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
void pause() {
    std::cout << "Press any key to continue...";
    flush();
    std::cin.get();
}
void footer() {
    std::cout << "\nSurab Parajuli\nSection: A, Roll: 34" << std::endl;
    pause();
}
}