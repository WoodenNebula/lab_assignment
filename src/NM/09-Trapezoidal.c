#include <stdio.h>
#include "commons.h"

int main() {
    Coeff y;
    promptEquation(&y);

    float x_lower, x_upper;
    printf("Enter lower and upper limit\n");
    scanf("%f%f", &x_lower, &x_upper);

    float h = x_upper - x_lower;
    float f_upper = f(y, x_upper);
    float f_lower = f(y, x_lower);

    float I = h * (f_upper + f_lower) / 2;
    printf("Value = %f", I);

    footer();
    return 0;
}
