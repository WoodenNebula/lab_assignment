#include<stdio.h>
#include<math.h>

#include "commons.h"

#define E 0.01

int main() {
    Coeff coeff;
    promptEquation(&coeff);
    printEquation(&coeff);

    float x_lower, x_upper;
    promptTwoGuess(&x_lower, &x_upper);

    float f_lower = f(coeff, x_lower);
    float f_upper = f(coeff, x_upper);

    float x_mid = (x_lower + x_upper) / 2;
    float f_mid = f(coeff, x_mid);


    if (f_lower * f_upper > 0) { abortOnError("Non bracketing guess"); }

    float root;

    int itr = 1;
    while (1) {
        f_lower = f(coeff, x_lower);
        f_upper = f(coeff, x_upper);

        x_mid = (x_lower + x_upper) / 2;
        f_mid = f(coeff, x_mid);

        float prod = f_lower * f_mid;

        // trace(itr, x_lower, x_upper, x_mid, f_lower, f_upper, f_mid, prod);
        if (prod == 0) {
            root = x_mid;
            break;
        }
        else if (prod < 0) { x_upper = x_mid; }
        else { x_lower = x_mid; }

        float e = fabs((x_lower - x_upper) / x_upper);

        if (e < E) {
            root = x_mid;
            break;
        }
        itr++;
    }
    printf("Root=%.3f\n", root);

    footer();
    return 0;
}
