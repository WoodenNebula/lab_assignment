#include <stdio.h>
#include <math.h>
#include "commons.h"

int main() {
    float x = 0, y = 0, z = 0, x_old, y_old, z_old, e1, e2, e3, e;
    int count = 1;

    printf("Enter tolerable error: ");
    scanf("%f", &e);

    printf("\nCount\tx\ty\tz\n");
    do {
        x_old = x;
        y_old = y;
        z_old = z;

        /*  Assumed equation:
            20x + y - 2z = 17
            3x + 20y - z = -18
            2x - 3y + 20z = 25
        */

        x = (17 - y + 2 * z) / 20;
        y = (-18 - 3 * x + z) / 20;
        z = (25 - 2 * x + 3 * y) / 20;

        printf("%d\t%0.4f\t%0.4f\t%0.4f\n", count, x, y, z);

        e1 = fabs(x - x_old);
        e2 = fabs(y - y_old);
        e3 = fabs(z - z_old);
        count++;
    } while (e1 > e || e2 > e || e3 > e);

    printf("\nSolution: x = %0.3f, y = %0.3f, z = %0.3f\n", x, y, z);

    footer();
    return 0;
}
