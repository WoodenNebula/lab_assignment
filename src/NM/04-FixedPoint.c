#include <stdio.h>
#include "commons.h"

#define E 0.001

int main() {
    printf("After arraging for x = f(x), ");
    Coeff g;
    promptEquation(&g);
    printEquation(&g);

    float x0;
    promptOneGuess(&x0);

    float root;
    int itr = 1;
    while (1) {
        float x = f(g, x0);
        float e = fabs(x - x0);
        // printf("x%d=%f\n", itr, x0);
        if (e < E) {
            root = x;
            break;
        }
        x0 = x;
        itr++;
    }
    printf("Root=%.3f\n", root);

    footer();
    return 0;
}
