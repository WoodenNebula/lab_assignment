/* Algorithm (Floyd-Warshall - Dynamic Programming):
1. Start
2. Initialise dist[i][j]:
   - 0          if i == j
   - weight(i,j) if a direct edge (i->j) exists
   - INF         otherwise
3. For each intermediate vertex k from 0 to n-1:
      For each source vertex i from 0 to n-1:
         For each destination vertex j from 0 to n-1:
            If dist[i][k] + dist[k][j] < dist[i][j]:
               dist[i][j] = dist[i][k] + dist[k][j]
   (Relax all edges through k as an intermediate node.)
4. Print the resulting all-pairs shortest path matrix.
5. End
*/

#include <climits>
#include <iomanip>
#include <iostream>
#include <vector>

#include "src/commons.hpp"

const int INF = INT_MAX / 2;

void floydWarshall(std::vector<std::vector<int>>& dist) {
    int n = (int)dist.size();

    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
}

void printMatrix(const std::vector<std::vector<int>>& dist) {
    int n = (int)dist.size();

    std::cout << "All-Pairs Shortest Paths:\n\n";
    std::cout << "     ";
    for (int j = 0; j < n; j++)
        std::cout << std::setw(5) << "V" + std::to_string(j);
    std::cout << "\n";

    for (int i = 0; i < n; i++) {
        std::cout << std::setw(3) << "V" + std::to_string(i) << "  ";
        for (int j = 0; j < n; j++) {
            if (dist[i][j] == INF)
                std::cout << std::setw(5) << "INF";
            else
                std::cout << std::setw(5) << dist[i][j];
        }
        std::cout << "\n";
    }
}

int main() {
    Header("FLOYD-WARSHALL");

    // 4-vertex weighted directed graph; INF = no direct edge
    std::vector<std::vector<int>> dist = {
        { 0, 3, INF, 7 },
        { 8, 0, 2, INF },
        { 5, INF, 0, 1 },
        { 2, INF, INF, 0 }
    };

    floydWarshall(dist);
    printMatrix(dist);

    Footer();
    return 0;
}
