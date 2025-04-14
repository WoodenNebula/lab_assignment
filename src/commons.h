#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>

#define MAX_SIZE 100

typedef struct {
    int* arr;
    size_t size;
} Array;


Array promptArray() {
    Array array = { NULL, 0 };
    printf("Size of Array: ");
    scanf("%zu", &array.size);

    array.arr = (int*)malloc(array.size * sizeof(int));
    if (array.arr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    printf("Enter %zu elements:\n", array.size);
    for (size_t i = 0; i < array.size; i++) {
        scanf("%d", &array.arr[i]);
    }

    return array;
}

void printArray(const Array* array) {
    printf("{ ");
    for (size_t i = 0; i < array->size - 1; i++) {
        printf("%d, ", array->arr[i]);
    }
    printf("%d }\n", array->arr[array->size - 1]);
}

void footer() {
    printf("\nSurab Parajuli\nSection: A, Roll: 34\n");
    system("pause");
}

void abortOnError(const char* msg) {
    printf("\e[1;91m");
    printf(msg);
    printf("\e[0m");
    footer();
    exit(EXIT_FAILURE);
}


/// Numerical Method
// Assuming a cubic equation i.e. f(x) = ax³ + bx² + cx + d
typedef struct {
    int a;
    int b;
    int c;
    int d;
}Coeff;

void promptEquation(Coeff* coeff) {
    setlocale(LC_CTYPE, "");
    wprintf(L"Enter coefficients for the equation: f(x) = ax³ + bx² + cx + d\n");
    printf("a = ");
    scanf("%d", &(coeff->a));
    printf("b = ");
    scanf("%d", &(coeff->b));
    printf("c = ");
    scanf("%d", &(coeff->c));
    printf("d = ");
    scanf("%d", &(coeff->d));
}

void promptOneGuess(float* x) {
    printf("Enter initial guess:\n");
    scanf("%f", x);
}

void promptTwoGuess(float* x_lower, float* x_upper) {
    printf("Enter two initial guesses:\n");
    printf("lower = ");
    scanf("%f", x_lower);

    printf("upper = ");
    scanf("%f", x_upper);
}

char sign(float d) { return d < 0 ? '-' : '+'; }

void printEquation(Coeff* coeff) {
    printf("f(x) =");
    if (coeff->a != 0) {
        wprintf(L" %c %dx³", sign(coeff->a), abs(coeff->a));
    }
    if (coeff->b != 0) {
        wprintf(L" %c %dx²", sign(coeff->b), abs(coeff->b));
    }
    if (coeff->c != 0) {
        printf(" %c %dx", sign(coeff->c), abs(coeff->c));
    }
    if (coeff->d != 0) {
        printf(" %c %d", sign(coeff->d), abs(coeff->d));
    }
    printf("\n");
}

void trace(int i, float x_lower, float x_upper, float x_mid, float f_lower, float f_upper, float f_mid, float prod) {
    printf("----------TRACE (%d)---------\n", i);
    printf("Lower (%.3f, %.3f)\t", x_lower, f_lower);
    printf("Upper (%.3f, %.3f)\n", x_upper, f_upper);
    printf("\t\tMid (%.3f, %.3f)\n", x_mid, f_mid);

    printf("Prod = %c %.3f\n", sign(prod), prod);
    printf("==========TRACE=========\n\n", i);
}
float f(Coeff coeff, float x) {
    float y = coeff.a * powf(x, 3) + coeff.b * (x * x) + coeff.c * x + coeff.d;
    // printf("____(%d, %f)____\n", x, y);
    return y;
}