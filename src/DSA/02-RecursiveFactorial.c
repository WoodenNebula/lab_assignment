/* Algorithm:
1. Start
2. Define a recursive function `factorial` that takes an integer `n` as input.
3. If `n` is 0 or 1, return 1 (base case).
4. Otherwise, return `n * factorial(n - 1)` (recursive case).
5. In the `main` function:
a. Take an integer input `n` from the user.
b. Call the `factorial` function with `n` and store the result.
c. Print the result.
6. End
*/
#include <stdio.h>
#include <stdlib.h>

int factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

int main() {
    int n;
    printf("n = ");
    scanf("%d", &n);

    printf("Factorial of %d is %d\n", n, factorial(n));

    printf("\nSurab Parajuli\nSection: A, Roll: 34\n");
    system("pause");
    return 0;
}