#include <stdio.h>
#include "commons.h"

#define fxy(x,y) 2*(y)/(x)

int main() {
    float x0, y0;
    printf("Enter initial x and y:\n");
    scanf("%f%f", &x0, &y0);

    float x = x0;
    float y = y0;

    float xp;
    printf("Evaluate at x = ");
    scanf("%f", &xp);

    float h;
    printf("Step(h) = ");
    scanf("%f", &h);

    for (x = x0; x < xp; x += h) {
        float m1 = fxy(x, y);
        float m2 = fxy(x + 1 / 2.0 * h, y + 1 / 2.0 * h * m1);
        float m3 = fxy(x + 1 / 2.0 * h, y + 1 / 2.0 * h * m2);
        float m4 = fxy(x + h, y + h * m3);
        y = y + (m1 + 2 * m2 + 2 * m2 + m4) * h / 6;
    }

    printf("Function value at x %f =%f", x, y);
    footer();
    return 0;
}