#include <stdio.h>
#include <math.h>
#include "commons.h"

double gx(int x, int y) { return (double)(2 * x * x * y * y); }

#define E 0.0001

void solvePoisson(Matrix* grid, double h) {
    double maxError;
    Matrix newGrid = *grid; // Temporary grid for updates

    do {
        maxError = 0.0;

        for (int r = 1; r < grid->row - 1; r++) {
            for (int c = 1; c < grid->col - 1; c++) {
                newGrid.data[r][c] = 0.25 * (grid->data[r - 1][c] + grid->data[r + 1][c] +
                    grid->data[r][c - 1] + grid->data[r][c + 1] - (h * h * gx(r, c)));

                double e = fabs(newGrid.data[r][c] - grid->data[r][c]);
                if (e > maxError) { maxError = e; }
            }
        }

        // Copy the new grid to the old grid
        for (int i = 1; i < grid->row - 1; i++) {
            for (int j = 1; j < grid->col - 1; j++) {
                grid->data[i][j] = newGrid.data[i][j];
            }
        }
    } while (maxError > E);
}

int main() {
    Matrix grid;

    // Input grid dimensions
    inputMatrixDimensions(&grid, "Enter plate and grid dimensions (plate grid):");
    grid.row += 2; // Adding boundary rows
    grid.col += 2; // Adding boundary columns

    struct Temps {
        double left; double right;
        double bottom; double top;
    }t;
    // }t = { 75, 100, 300, 50 };

    printf("Boundary values:\n(left, right, bottom, top)\n");
    scanf("%lf%lf%lf%lf", &t.left, &t.right, &t.bottom, &t.top);

    // Initialize the grid with boundary conditions
    for (int r = 0; r < grid.row; r++) {
        for (int c = 0; c < grid.col; c++) {
            int isTop = r == 0, isBottom = r == grid.row - 1;
            int isLeft = c == 0, isRight = c == grid.col - 1;
            int isDiagonal = (r == c || c == grid.row - 1 - r);

            double term = 0.0;
            if (!isDiagonal) {
                if (isTop) { term = t.top; }
                if (isBottom) { term = t.bottom; }
                if (isLeft) { term = t.left; }
                if (isRight) { term = t.right; }
            }

            if (!isTop && !isBottom && !isRight && !isLeft) {
                term = grid.data[grid.row - 1][c];
            }

            grid.data[r][c] = term;
        }
    }

    double h = 1;
    printf("Enter step(h):\n");
    scanf("%lf", &h);

    // Solve Poisson's equation
    solvePoisson(&grid, h);

    printMatrix(&grid, "Final grid after solving Poisson's equation:");

    footer();
    return 0;
}