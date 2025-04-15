#include <stdio.h>
#include <math.h>
#include "commons.h"

int main() {
    Matrix augMat;
    // augMat = (Matrix){
        // .row = 4,
        // .col = 5,
        // .data = {
            // { 1, 1, 2, 0, 1 },
        // { 2, -1, 0, 1, -2 },
        // { 1, -1, -1, -2, 4 },
        // { 2, -1, 2, -1, 0 }
    // }
    // };
    // printMatrix(&augMat, "Augmented Matrix is: ");
    inputMatrixDimensions(&augMat, "Enter (coeff) square matrix dimensions: row = col");
    augMat.col = augMat.row + 1;
    inputMatrix(&augMat, "Enter data for Augmented Matrix: ");

    // Elimination
    for (int row = 0; row < augMat.row; row++) {
        double pivot = augMat.data[row][row];

        // Normalize pivot row
        for (int col = 0; col < augMat.col; col++) {
            augMat.data[row][col] = augMat.data[row][col] / pivot;
        }

        for (int i = 0; i < augMat.row; i++) {
            double term = augMat.data[i][row];
            if (row == i) { continue; }
            for (int j = 0; j < augMat.col; j++) {
                augMat.data[i][j] = augMat.data[i][j] - augMat.data[row][j] * term;
            }
        }

        // printMatrix(&augMat, "\n====Augmented====\n");
    }


    Matrix sol = { .row = augMat.row, .col = 1 };
    for (int i = 0; i < sol.row; i++) {
        sol.data[i][0] = augMat.data[i][augMat.col - 1];
        // printf("| %lf |\n", augMat.data[i][augMat.col - 1]);
    }

    printMatrix(&sol, "Solution: ");

    footer();
    return 0;
}
