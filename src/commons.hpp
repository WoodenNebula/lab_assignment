#pragma once
#include <ios>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#define RED "\033[91m"
#define GREEN "\033[92m"
#define RESET "\033[0m"

#if (LOG_LEVEL == 1)
#define LOG(x) printf("\n[LOG] : %s\n", x)
#else
#define LOG(x)
#endif

void Header(std::string_view Title) {
    std::cout << "=== " << std::uppercase << Title << " ===" << std::endl;
}

void Footer() {
    std::cout << "\n====================\n"
        << "Surab Parajuli\n"
        << "Section: A, 5th-Sem\n"
        << "Roll: 34, Symbol No.: 80010139\n"
        << "====================\n";
#ifndef __linux
    //  syst("read -p \"Press any key to continue...\"");
    system("pause");
#endif
}

void abortOnError(std::string_view msg) {
    std::cout << RED << msg << RESET;
    Footer();
    exit(EXIT_FAILURE);
}

// template <class ContainerType>
// std::string ToString(ContainerType Container) {
//     std::string containerStr = "[ ";
//     for (const auto x : Container) {
//         containerStr += x + ", ";
//     }
//
//     containerStr.pop_back();
//     containerStr.pop_back();
//     containerStr += " ]";
//
//     return containerStr;
// }


namespace Surab
{
using Mat = std::vector<std::vector<double>>;

template <class ContainerType>
std::string ToString(const ContainerType& Container) {
    std::string containerStr = "[ ";
    for (const auto x : Container) {
        containerStr += std::to_string(x) + ", ";
    }

    containerStr.pop_back();
    containerStr.pop_back();
    containerStr += " ]";

    return containerStr;
}
}

std::istream& operator>>(std::istream& in, Surab::Mat& matrix) {
    for (size_t row = 0; row < matrix.size(); row++) {
        std::cout << "  row[" << row + 1 << "] <- ";
        for (size_t col = 0; col < matrix[row].size(); col++) {
            in >> matrix[row][col];
        }
    }
    std::cout << "\n";
    return in;
}

std::ostream& operator<<(std::ostream& out, const Surab::Mat& matrix) {
    for (size_t row = 0; row < matrix.size(); row++) {
        out << "\t" << Surab::ToString(matrix[row]) << "\n";
    }
    return out;
}