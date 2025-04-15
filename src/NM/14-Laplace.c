#include <stdio.h>
#include <math.h>
#include "commons.h"

#define E 0.0001

void solveLaplace(Matrix* grid, int maxIterations) {
    int rows = grid->row;
    int cols = grid->col;
    double maxError;

    for (int iter = 0; iter < maxIterations; iter++) {
        maxError = 0.0;

        for (int r = 1; r < rows - 1; r++) {
            for (int c = 1; c < cols - 1; c++) {
                double oldValue = grid->data[r][c];
                grid->data[r][c] = 0.25 * (grid->data[r - 1][c] + grid->data[r + 1][c] +
                    grid->data[r][c - 1] + grid->data[r][c + 1]);
                double error = fabs(grid->data[r][c] - oldValue);
                if (error > maxError) { maxError = error; }
            }
        }

        // printf("Iteration %d: Max Error = %.6lf\n", iter + 1, maxError);
        if (maxError < E) { break; }
    }
}

int main() {
    Matrix grid;
    inputMatrixDimensions(&grid, "Enter grid dimensions (rows cols):");
    grid.row += 2; // Adding boundary rows
    grid.col += 2; // Adding boundary columns

    struct Temps {
        double left;
        double right;
        double bottom;
        double top;
    }t = { 75, 100, 300, 50 };

    // printf("Boundary values:\n(left, right, bottom, top)\n");
    // scanf("%lf%lf%lf%lf", &t.left, &t.right, &t.bottom, &t.top);

    // Initialize the grid with boundary conditions
    for (int r = 0; r < grid.row; r++) {
        for (int c = 0; c < grid.col; c++) {
            int isTop = r == 0, isBottom = r == grid.row - 1;
            int isLeft = c == 0, isRight = c == grid.col - 1;
            int isDiagonal = (r == c || c == grid.row - 1 - r);

            double term = 0.0;
            if (!isDiagonal) {
                if (isTop) { term = t.top; }
                else if (isBottom) { term = t.bottom; }
                else if (isLeft) { term = t.left; }
                else if (isRight) { term = t.right; }
            }

            grid.data[r][c] = term;
        }
    }

    int maxIterations = 100;
    // printf("Enter maximum number of iterations: ");
    // scanf("%d", &maxIterations);

    solveLaplace(&grid, maxIterations);

    printMatrix(&grid, "Final grid after solving Laplace's equation:");

    footer();
    return 0;
}