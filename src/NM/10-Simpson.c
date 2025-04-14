#include <stdio.h>
#include "commons.h"

int main() {
    Coeff y;
    promptEquation(&y);

    float x_lower, x_upper;
    printf("Enter lower and upper limit\n");
    scanf("%f%f", &x_lower, &x_upper);

    float h = (x_upper - x_lower) / 2;
    float f_upper = f(y, x_upper);
    float f_lower = f(y, x_lower);

    float f_dh = f(y, x_lower + h);

    float I = (h / 3) * (f_lower + 4 * f_dh + f_upper);
    printf("Value = %f", I);

    footer();
    return 0;
}
