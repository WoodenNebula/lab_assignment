/* Compute factorial of integer N using while loop */

#include <stdio.h>

int main(int argc, char *argv[]) {
    int n;
    puts("Enter positive integer n:");
    scanf("%d", &n);

    if (n == 0)
        puts("0! = 1");
    else {
        long long int fact = 1;
        int i = 1;

        while( i <= n) {
            fact *= i;
            i++;
        }

        printf("%d! = %lld", n, fact);
    }

    return 0;
}