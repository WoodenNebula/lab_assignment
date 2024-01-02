/* Compute the roots of the quadratic equation ax² + bx + c = 0 for given coefficient input a, b and c */
#include <stdio.h>
#include <math.h>


int main(int argc, char *argv[]) {
    int a, b, c;
    puts("Enter integer values of a, b and c respectively:");
    scanf("%d %d %d", &a, &b, &c);
    
    int disc = b * b - 4 * a * c;

    if (disc > 0)
    {
        float root_first, root_second;
        root_first = (float)(-b + sqrt(disc)) / (2 * a);
        root_second = (float)(-b - sqrt(disc)) / (2 * a);
        printf("Roots are: %.3f and %.3f", root_first, root_second);
    }
    else if(disc < 0)
    {
        puts("Imaginary Roots are out of the scope!!!");
    }
    else
    {
        float root = (float)(-b) / (2 * a);
        printf("Equal roots = %.2f", root);
    }

    return 0;
}