#include <stdio.h>
#include <math.h>
#include "commons.h"

int main() {
    Matrix coeff;
    // coeff = (Matrix){
    //     .row = 3,
    //     .col = 3,
    //     .data = {
    //         { 2, 3, 1 },
    //     { 1, 2, 3 },
    //     { 3, 1, 2 } }
    // };
    inputMatrixDimensions(&coeff, "Enter square matrix dimensions: row = col");
    coeff.col = coeff.row;
    inputMatrix(&coeff, "Enter Data(s): ");

    Matrix rhs = { .row = coeff.row, .col = 1 };
    // rhs = (Matrix){
    //     .row = 3, .col = 1,
    //     .data = {
    //         { 9 },
    //     { 6 },
    //     { 8 } }
    // };
    inputMatrix(&rhs, "Enter RHS:");

    Matrix u = { .row = coeff.row, .col = coeff.col };
    Matrix l = { .row = coeff.row, .col = coeff.col };
    // init
    for (int i = 0; i < u.row; i++) {
        for (int j = 0; j < u.col; j++) {
            u.data[i][j] = 0.0;
            l.data[i][j] = 0.0;
        }
    }

    // Computing L and U matrices
    for (int i = 0; i < coeff.row; i++) {
        u.data[0][i] = coeff.data[0][i];

        l.data[i][0] = coeff.data[i][0] / u.data[0][0];
        l.data[i][i] = 1;
    }

    for (int r = 1; r < coeff.row; r++) {
        for (int c = 1; c <= r; c++) {
            double sum = 0.0;
            for (int k = 0; k < c; k++) {
                sum += (l.data[c][k] * u.data[k][r]);
            }
            u.data[c][r] = coeff.data[c][r] - sum;
        }
        for (int c = r + 1; c < coeff.row; c++) {
            double sum = 0.0;
            for (int k = 0; k < r; k++) {
                sum += l.data[c][k] * u.data[k][r];
            }
            l.data[c][r] = (coeff.data[c][r] - sum) / u.data[r][r];
        }
    }

    // Solving for z using Forward Substitution
    Matrix z = { .row = coeff.row, .col = 1 };
    z.data[0][0] = rhs.data[0][0];
    for (int i = 1;i < coeff.row;i++) {
        double sum = 0.0;
        for (int j = 0;j < i;j++) {
            sum += (l.data[i][j] * z.data[j][0]);
        }
        z.data[i][0] = rhs.data[i][0] - sum;
    }

    // Backward Substitution
    Matrix  x = { .row = coeff.row, .col = 1 };
    x.data[x.row - 1][0] = z.data[x.row - 1][0] / u.data[x.row - 1][x.row - 1];
    for (int i = x.row - 2;i >= 0;i--) {
        double sum = 0.0;
        for (int j = i + 1;j < x.row;j++) {
            sum += (u.data[i][j] * x.data[j][0]);
        }
        x.data[i][0] = (z.data[i][0] - sum) / u.data[i][i];
    }

    printMatrix(&x, "Solution: ");

    footer();
    return 0;
}
