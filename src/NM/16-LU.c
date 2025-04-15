#include <stdio.h>
#include <math.h>
#include "commons.h"

int main() {
    Matrix coeff;
    coeff = (Matrix){
        .row = 3,
        .col = 3,
        .data = {
            { 20, 15, 10 },
        { -3, -2.249, 7 },
        { 5, 1, 3 } }
    };
    // inputMatrixDimensions(&coeff, "Enter square matrix dimensions: row = col");
    // coeff.col = coeff.row;
    // inputMatrix(&coeff, "Enter Data(s): ");

    Matrix rhs = { .row = coeff.row, .col = 1 };
    rhs = (Matrix){
        .row = 3, .col = 1,
        .data = {
            { 45 },
        { 1.751 },
        { 9 } }
    };
    // inputMatrix(&rhs, "Enter RHS:");

    Matrix u = { .row = coeff.row, .col = coeff.col };
    Matrix l = { .row = coeff.row, .col = coeff.col };

    // Computing L and U matrices
    for (int col = 0;col < coeff.row;col++) {
        u.data[0][col] = coeff.data[0][col];
    }

    for (int row = 0; row < coeff.row; row++)
        l.data[row][row] = 1;

    for (int i = 1;i < coeff.row; i++)
        l.data[i][0] = coeff.data[i][0] / u.data[0][0];

    for (int j = 1;j < coeff.row;j++) {
        for (int i = 1;i <= j;i++) {
            double sum = 0.0;
            for (int k = 0;k <= i - 1;k++) {
                sum += (l.data[i][k] * u.data[k][j]);
            }
            u.data[i][j] = coeff.data[i][j] - sum;
        }
        for (int i = j + 1;i < coeff.row;i++) {
            double sum = 0.0;
            for (int k = 0;k <= j - 1;k++) {
                sum += l.data[i][k] * u.data[k][j];
            }
            l.data[i][j] = (coeff.data[i][j] - sum) / u.data[j][j];
        }
    }
    // Solving for z using Forward Substitution
    Matrix b = { .row = coeff.row, .col = 1 };
    Matrix z = { .row = coeff.row, .col = 1 };
    z.data[0][0] = b.data[0][0];
    for (int i = 1;i < coeff.row;i++) {
        double sum = 0.0;
        for (int j = 0;j < i;j++) {
            sum += (l.data[i][j] * z.data[j][0]);
        }
        z.data[i][0] = b.data[i][0] - sum;
        sum = 0;
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
