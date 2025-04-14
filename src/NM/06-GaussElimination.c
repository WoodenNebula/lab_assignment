#include <stdio.h>
#include <math.h>
#include "commons.h"

#define MAX 10

int main() {
    Matrix coeff;
    // coeff = (Matrix){
    //     .row = 3,
    //     .col = 3,
    //     .data = {
    //         { 20, 15, 10 },
    //     { -3, -2.249, 7 },
    //     { 5, 1, 3 } }
    // };
    inputMatrixDimensions(&coeff, "Enter square matrix dimensions: row = col");
    coeff.col = coeff.row;
    inputMatrix(&coeff, "Enter Data(s): ");

    Matrix rhs = { .row = coeff.row, .col = 1 };
    // rhs = (Matrix){
    //     .row = 3, .col = 1,
    //     .data = {
    //         { 45 },
    //     { 1.751 },
    //     { 9 } }
    // };
    inputMatrix(&rhs, "Enter RHS:");



    // Forward Elimination
    for (int i = 0; i < coeff.row - 1; i++) {
        double pivot = coeff.data[i][i];
        if (fabs(pivot) < FLOAT_TOLERANCE) { abortOnError("Divide By Zero!"); }

        for (int ii = i + 1; ii < coeff.row; ii++) {
            double term = coeff.data[ii][i] / pivot;
            for (int iii = 0; iii < coeff.row; iii++) {
                coeff.data[ii][iii] = coeff.data[ii][iii] - coeff.data[i][iii] * term;
            }
            rhs.data[ii][0] = rhs.data[ii][0] - rhs.data[i][0] * term;
        }
    }


    // Backward Substitution
    Matrix x = { .row = coeff.row, .col = 1 };
    // x₃ = B₃/C₃₃
    x.data[x.row - 1][0] = rhs.data[rhs.row - 1][0] / coeff.data[coeff.row - 1][coeff.row - 1];

    for (int xi = x.row - 2; xi >= 0; xi--) {
        double sum = 0;
        for (int ii = xi + 1; ii < coeff.row; ii++) {
            sum += coeff.data[xi][ii] * x.data[ii][0];
        }
        x.data[xi][0] = (rhs.data[xi][0] - sum) / coeff.data[xi][xi];
    }
    printMatrix(&x, "Solution: ");

    footer();
    return 0;
}
