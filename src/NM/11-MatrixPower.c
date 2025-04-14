#include <stdio.h>
#include "commons.h"

#define ERROR_TOLERANCE 0.000001

int main() {
    Matrix mat;
    // mat = (Matrix){
        // .row = 3,
        // .col = 3,
        // .data = {
            // { 1, 2, 0 },
        // { 2, 1, 0 },
        // { 0, 0, -1 }
    // }
    // };
    inputMatrixDimensions(&mat, "Enter (mat) square matrix dimensions: row = col");
    mat.col = mat.row;
    inputMatrix(&mat, "Enter Data(s): ");

    double eigenvalue = 0.0, prev_eigenvalue = 0.0;
    Matrix x = { .row = mat.row, .col = 1 };
    // x = (Matrix){
        // .row = 3, .col = 1,
        // .data = {
            // { 0 },
        // { 1 },
        // { 0 } }
    // };
    inputMatrix(&x, "Enter Guess vector:");

    while (1) {
        Matrix prod = { .row = mat.row, .col = 1 };
        // vector and matrix multiplication
        // prod = mat * x
        for (int i = 0; i < mat.row; i++) {
            prod.data[i][0] = 0.0;
            for (int j = 0; j < mat.col; j++) {
                prod.data[i][0] += mat.data[i][j] * x.data[j][0];
            }
        }

        // Find the largest value in prod (approximation of eigenvalue)
        eigenvalue = fabs(prod.data[0][0]);
        for (int i = 1; i < prod.row; i++) {
            if (fabs(prod.data[i][0]) > eigenvalue) {
                eigenvalue = fabs(prod.data[i][0]);
            }
        }

        // Normalize prod
        for (int i = 0; i < prod.row; i++) {
            prod.data[i][0] /= eigenvalue;
        }

        // Check for convergence
        if (fabs(eigenvalue - prev_eigenvalue) < ERROR_TOLERANCE) {
            break;
        }

        // Update x and prev_eigenvalue for the next iteration
        for (int i = 0; i < x.row; i++) {
            x.data[i][0] = prod.data[i][0];
        }
        prev_eigenvalue = eigenvalue;
    }

    printf("\nLargest Eigenvalue: %.6lf\n", eigenvalue);
    footer();
    return 0;
}
