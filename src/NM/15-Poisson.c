#include "commons.h"

#define MAX 10

// Function to initialize the grid
void initializeGrid(Matrix* grid, double boundaryValue) {
    for (int i = 0; i < grid->row; i++) {
        for (int j = 0; j < grid->col; j++) {
            if (i == 0 || i == grid->row - 1 || j == 0 || j == grid->col - 1) {
                grid->data[i][j] = boundaryValue; // Boundary condition
            }
            else {
                grid->data[i][j] = 0.0; // Initial guess for interior points
            }
        }
    }
}

// Function to solve Poisson's equation using the finite difference method
void solvePoisson(Matrix* grid, double tolerance) {
    double maxError;
    Matrix newGrid = *grid; // Temporary grid for updates

    do {
        maxError = 0.0;

        // Update the grid using the finite difference method
        for (int i = 1; i < grid->row - 1; i++) {
            for (int j = 1; j < grid->col - 1; j++) {
                newGrid.data[i][j] = 0.25 * (grid->data[i - 1][j] + grid->data[i + 1][j] +
                    grid->data[i][j - 1] + grid->data[i][j + 1]);

// Calculate the maximum error
                double error = fabs(newGrid.data[i][j] - grid->data[i][j]);
                if (error > maxError) {
                    maxError = error;
                }
            }
        }

        // Copy the new grid to the old grid
        for (int i = 1; i < grid->row - 1; i++) {
            for (int j = 1; j < grid->col - 1; j++) {
                grid->data[i][j] = newGrid.data[i][j];
            }
        }
    } while (maxError > tolerance);
}

int main() {
    Matrix grid;
    double boundaryValue, tolerance;

    // Input grid dimensions
    inputMatrixDimensions(&grid, "Enter the grid dimensions (rows and columns):");
    if (grid.row > MAX || grid.col > MAX) {
        abortOnError("Grid size exceeds the maximum limit.\n");
    }

    // Input boundary value
    printf("Enter the boundary value: ");
    scanf("%lf", &boundaryValue);

    // Input tolerance
    printf("Enter the tolerance: ");
    scanf("%lf", &tolerance);

    // Initialize the grid
    initializeGrid(&grid, boundaryValue);

    // Print the initial grid
    printMatrix(&grid, "Initial Grid:");

    // Solve Poisson's equation
    solvePoisson(&grid, tolerance);

    // Print the solution grid
    printMatrix(&grid, "Solution Grid:");

    footer();
    return 0;
}