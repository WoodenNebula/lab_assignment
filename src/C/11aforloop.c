/* Compute factorial of integer N using for loop */

#include <stdio.h>

int main(int argc, char *argv[]) {
    int n;
    puts("Enter positive integer n:");
    scanf("%d", &n);

    if (n == 0)
        puts("0! = 1");
    else {
        long long int fact = 1;
        
        for(int i = 1; i <= n; i ++) {
            fact *= i;
        }

        printf("%d! = %lld", n, fact);
    }

    return 0;
}