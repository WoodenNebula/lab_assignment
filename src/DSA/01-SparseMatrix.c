/*
Algorithm:
1. Start
2. Define the number of rows (`rows`) and columns (`cols`) of the matrix.
3. Initialize the 2D array `matrix` with given values (including both zero and non-zero elements).
4. Use nested loops to iterate through each element of the matrix.
    4.1. If the element is non-zero:
        4.1.1. Print its row index, column index, and value in the format `(row, column, value)`.
5. Wait for User Input
6. End

*/
#include <stdio.h>
#include <stdlib.h>

int main() {
    int rows = 4, cols = 5;
    int matrix[4][5] = {
        { 0, 0, 1, 0, 0 },
        { 0, 0, 6, 7, 0 },
        { 0, 1, 0, 0, 0 },
        { 0, 1, 8, 0, 2 }
    };

    printf("Original Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\nSparse Matrix Representation\n(row, column, value):\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] != 0) {
                printf("(%d, %d) = %d\n", i + 1, j + 1, matrix[i][j]);
            }
        }
    }

    printf("\nSurab Parajuli\nSection: A, Roll: 34\n");
    system("pause");
    return 0;
}