/* Algorithm (Matrix Chain Multiplication - Dynamic Programming):
1. Start
2. Given dimensions array p[0..n] where matrix i has size p[i-1] x p[i].
3. Create cost table  m[1..n][1..n] (min scalar multiplications), init 0.
   Create split table s[1..n][1..n] (optimal split point).
4. For chain length l = 2 to n:
      For i = 1 to n-l+1:
         j = i + l - 1
         m[i][j] = INF
         For k = i to j-1:
            q = m[i][k] + m[k+1][j] + p[i-1] * p[k] * p[j]
            If q < m[i][j]:
               m[i][j] = q
               s[i][j] = k
5. m[1][n] is the minimum number of scalar multiplications.
6. Recover the optimal parenthesization by tracing the s table.
7. End
*/

#include <climits>
#include <iostream>
#include <vector>

#include "src/commons.hpp"

void printOptimalParens(const std::vector<std::vector<int>>& s, int i, int j) {
    if (i == j) {
        std::cout << "M" << i;
        return;
    }
    std::cout << "(";
    printOptimalParens(s, i, s[i][j]);
    printOptimalParens(s, s[i][j] + 1, j);
    std::cout << ")";
}

void matrixChainOrder(const std::vector<int>& p) {
    int n = (int)p.size() - 1;

    std::vector<std::vector<int>> m(n + 1, std::vector<int>(n + 1, 0));
    std::vector<std::vector<int>> s(n + 1, std::vector<int>(n + 1, 0));

    for (int l = 2; l <= n; l++) {              // l = chain length
        for (int i = 1; i <= n - l + 1; i++) {
            int j    = i + l - 1;
            m[i][j]  = INT_MAX;

            for (int k = i; k < j; k++) {
                int q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (q < m[i][j]) {
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }

    std::cout << "Minimum scalar multiplications: " << m[1][n] << std::endl;
    std::cout << "Optimal parenthesization:       ";
    printOptimalParens(s, 1, n);
    std::cout << std::endl;
}

int main() {
    Header("MATRIX CHAIN MULTIPLICATION");

    // Matrices: M1(30x35), M2(35x15), M3(15x5), M4(5x10), M5(10x20), M6(20x25)
    std::vector<int> dims = { 30, 35, 15, 5, 10, 20, 25 };

    std::cout << "Matrix dimensions:\n";
    for (int i = 0; i < (int)dims.size() - 1; i++)
        std::cout << "  M" << i + 1 << " = " << dims[i] << " x " << dims[i + 1] << "\n";
    std::cout << std::endl;

    matrixChainOrder(dims);

    Footer();
    return 0;
}
