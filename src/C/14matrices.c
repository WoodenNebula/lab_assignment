/* Input 2 3x3 matrices and find their sum and product */

#include <stdio.h>
#include <math.h>


void Multiply(int mat_one[][3][3], int mat_two[][3][3], int o_product[][3][3]);
void Sum(int mat_one[][3][3], int mat_two[][3][3], int o_sum[][3][3]);
void InputMatrix(int o_matrix[][3][3]);
void PrintMatrix(int matrix[][3][3]);


int main() {
    int matrixA[3][3] = { 0 }, matrixB[3][3] = { 0 };
    int sum[3][3] = { 0 }, product[3][3] = { 0 };

    puts("Input Matrix[1]:");
    InputMatrix(&matrixA);

    puts("\nInput Matrix[2]:");
    InputMatrix(&matrixB);


    puts("\n--------------------");

    Sum(&matrixA, &matrixB, &sum);
    Multiply(&matrixA, &matrixB, &product);

    puts("Sum is:");
    PrintMatrix(&sum);

    puts("\nProduct is:");
    PrintMatrix(&product);

    return 0;
}


void Multiply(int mat_one[][3][3], int mat_two[][3][3], int o_product[][3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++){
            int temp_prod = 0;
            (*o_product)[i][j] = 0;
            for (int column = 0, row = 0;  column < 3 || row < 3; column++, row++) {
                temp_prod = (*mat_one)[i][column] * (*mat_two)[row][j];
                (*o_product)[i][j] += temp_prod;
            }
        }
    }
}


void Sum(int mat_one[][3][3], int mat_two[][3][3], int o_sum[][3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++){
            (*o_sum)[i][j] = (*mat_one)[i][j] + (*mat_two)[i][j];
        }
    }
}


void PrintMatrix(int matrix[][3][3]){
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", (*matrix)[i][j]);
        }
        puts("");
    }
}


void InputMatrix(int o_matrix[][3][3]){
    for (int i = 0; i < 3; i++) {
        printf("\t\trow[%d] <- ", i + 1);
        for (int j = 0; j < 3; j++){
            scanf("%d", &(*o_matrix)[i][j]);
        }
    }
}