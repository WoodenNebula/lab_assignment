#include<stdio.h>
#include<math.h>

#include "commons.h"

#define E 0.1

int main() {
    Coeff y;
    promptEquation(&y);
    Coeff y_prime = {
        .a = 0,
        .b = 3 * y.a,
        .c = 2 * y.b,
        .d = y.c
    };
    printEquation(&y);
    printEquation(&y_prime);

    float x;
    promptOneGuess(&x);

    float root;
    int itr = 1;
    while (1) {
        float f1 = f(y, x);
        float f2 = f(y_prime, x);

        if (f2 == 0) { abortOnError("Divide by zero"); }
        if (itr > 50) { abortOnError("Oscillation"); }

        float xi = x - f1 / f2;
        float e = fabs(xi - x);
        // printf("x%d=%f\n", itr, x);
        if (e < E) {
            root = xi;
            break;
        }
        x = xi;
        itr++;
    }
    printf("Root=%.3f\n", root);

    footer();
    return 0;
}
