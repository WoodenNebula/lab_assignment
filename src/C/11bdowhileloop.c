/* Computation of a raised to power b using do while*/

#include <stdio.h>

int main(int argc, char *argv[]) {
    int base, exponent;
    puts("Enter positive base(a) and exponent(b) [1 <= values <= 2^32]:");
    scanf("%d %d", &base, &exponent);

    unsigned long long result = 1;

    if (base == 0 && exponent == 0) {
        puts("Indeterminate!");
        return 1;
    }
    else if(base == 0)
        result = 0;
    else if(exponent == 0)
        result = 1;
    else if(base < 0 ||exponent < 0) {
        puts("Negative numbers not allowed!");
        return 1;
    }
    else if(exponent > 0) {
        int i = 1;
        do {
            result *= base;
            i++;
        } while ( i <= exponent);
    }

    printf("%d raised to %d = %llu", base, exponent, result);

    return 0;
}