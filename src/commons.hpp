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

std::string ToString(const std::vector<int>& Container) {
    std::string containerStr = "[ ";
    for (const auto x : Container) {
        containerStr += std::to_string(x) + ", ";
    }

    containerStr.pop_back();
    containerStr.pop_back();
    containerStr += " ]";

    return containerStr;
}

#ifdef SURAB_MATRIX
#define FLOAT_TOLERANCE 0.000001
typedef struct {
    int row;
    int col;
    double data[10][10];
} Matrix;

void inputMatrixDimensions(Matrix* o_mat, const char* msg) {
    printf(msg);
    printf("\n");
    scanf("%d%d", &(o_mat->row), &(o_mat->col));
    printf("\n");
}

void inputMatrix(Matrix* o_mat, const char* msg) {
    printf(msg);
    printf("\n");
    for (int r = 0; r < o_mat->row; r++) {
        printf("  row[%d] <- ", r + 1);
        for (int c = 0; c < o_mat->col; c++) {
            scanf("%lf", &(o_mat->data)[r][c]);
        }
    }
    printf("\n");
}

void printMatrix(const Matrix* matrix, const char* msg) {
    printf(msg);
    printf("\n");
    for (int r = 0; r < matrix->row; r++) {
        printf("| ");
        for (int c = 0; c < matrix->col; c++) {
            printf("%.4lf ", (matrix->data)[r][c]);
        }
        printf("|\n");
    }
    printf("\n");
}
#endif  // SURAB_MATRIX
