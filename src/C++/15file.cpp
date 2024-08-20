#include <iostream>
#include <fstream>
#include <filesystem>
// #include "file.txt"

int main() {
    std::string path = ".\\src\\C++\\file.txt";

    {
        std::ifstream file(path);

        std::cout << "Reading per character:\n\n";

        while (file.good()) {
            char c;
            c = file.get();
            std::cout << c;
        }
        std::cout << std::endl;
    }

    {
        std::ifstream file(path);
        char line[256];

        std::cout << "\nReading per line:\n\n";

        while (file.getline(line, 256)) {
            std::cout << line << std::endl;
        }
        std::cout << std::endl;
    }

}