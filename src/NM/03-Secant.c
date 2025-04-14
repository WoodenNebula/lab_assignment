#include<stdio.h>
#include<math.h>

#include "commons.h"

#define E 0.001

int main() {
    Coeff y;
    promptEquation(&y);
    printEquation(&y);

    float x1, x2;
    promptTwoGuess(&x1, &x2);

    float root, x;
    int itr = 1;
    while (1) {
        float f1 = f(y, x1);
        float f2 = f(y, x2);

        x = x2 - ((x2 - x1) * f2) / (f2 - f1);

        float e = fabs(x - x1);
        // printf("x%d=%f\n", itr, x1);
        if (e < E) {
            root = x;
            break;
        }
        x1 = x2;
        x2 = x;
        itr++;
    }
    printf("Root=%.3f\n", root);

    footer();
    return 0;
}
