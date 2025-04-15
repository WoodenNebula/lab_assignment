#include "commons.h"

void solveLaplace(Matrix* grid, int maxIterations, double tolerance) {
    int rows = grid->row;
    int cols = grid->col;
    double maxError;

    printf("Starting Laplace solver...\n");
    for (int iter = 0; iter < maxIterations; iter++) {
        maxError = 0.0;

        for (int r = 1; r < rows - 1; r++) {
            for (int c = 1; c < cols - 1; c++) {
                double oldValue = grid->data[r][c];
                grid->data[r][c] = 0.25 * (grid->data[r - 1][c] + grid->data[r + 1][c] +
                    grid->data[r][c - 1] + grid->data[r][c + 1]);
                double error = fabs(grid->data[r][c] - oldValue);
                if (error > maxError) {
                    maxError = error;
                }
            }
        }

        printf("Iteration %d: Max Error = %.6lf\n", iter + 1, maxError);
        if (maxError < tolerance) {
            printf("Converged after %d iterations.\n", iter + 1);
            break;
        }
    }
}

int main() {
    Matrix grid;
    int maxIterations;
    double tolerance;

    printf("Laplace Equation Solver\n");
    inputMatrixDimensions(&grid, "Enter grid dimensions (rows cols):");
    grid.row += 2; // Adding boundary rows
    grid.col += 2; // Adding boundary columns

    // Initialize the grid with boundary conditions
    for (int r = 0; r < grid.row; r++) {
        for (int c = 0; c < grid.col; c++) {
            if (r == 0 || r == grid.row - 1 || c == 0 || c == grid.col - 1) {
                printf("Boundary value at (%d, %d): ", r, c);
                scanf("%lf", &grid.data[r][c]);
            }
            else {
                grid.data[r][c] = 0.0; // Initial guess for interior points
            }
        }
    }

    printf("Enter maximum number of iterations: ");
    scanf("%d", &maxIterations);

    printf("Enter tolerance for convergence: ");
    scanf("%lf", &tolerance);

    solveLaplace(&grid, maxIterations, tolerance);

    printMatrix(&grid, "Final grid after solving Laplace's equation:");

    footer();
    return 0;
}