#include <stdio.h>
#include <math.h>
#include "commons.h"

#define E 0.001
#define MAX 100

int main() {

    int n;
    printf("Number of Points:\n");
    scanf("%d", &n);

    float x[MAX], y[MAX];
    printf("Enter (x, f(x)):\n");
    for (int i = 0; i < n; i++) {
        scanf("%f%f", &(x[i]), &(y[i]));
    }

    int x_total = 0, y_total = 0, xy_total = 0, x_sqTotal = 0;
    for (int i = 0; i < n; i++) {
        x_total += x[i];
        y_total += y[i];
        xy_total += (x[i] * y[i]);
        x_sqTotal += (x[i] * x[i]);
    }

    // y = a + bx
    // b = (n∑xy - ∑x∑y) / (n∑x² - (∑x)²)
    float b = ((n * xy_total) - (x_total * y_total)) / (float)((n * x_sqTotal) - (x_total * x_total));
    // a = (∑y - b∑x)/n
    float a = (y_total - (b * x_total)) / n;

    printf("\nFitted Curve is: y = %.3f %c %.3fx", a, sign(b), fabs(b));

    footer();
    return 0;
}
