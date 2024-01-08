/* Write a program that inputs 2 integers and calculates the sum, difference, product, quotient and remainder applying different arithmetic operators between them */
#include <stdio.h>

int main(int argc, char *argv[]) {
    int varA, varB;
    puts("Enter two integer:");
    scanf("%d %d", &varA, &varB);

    int sum, diff, product, remainder;
    float quotient;

    sum = varA + varB;
    diff = varA - varB;
    quotient = (float)varA / varB;
    product = varA * varB;
    remainder = varA % varB;

    printf("Sum = %d\n", sum);
    printf("Difference = %d\n", diff);
    printf("Product = %d\n", product);
    printf("Quotient = %.2f\n", quotient);
    printf("Remainder = %d\n", remainder);

    return 0;
}