/* Swap two numbers defining a function swap() */

#include <stdio.h>

void swap_var(int* x, int* y) {
    if(x == y) {
        return;
    }

    *x = *x ^ *y;
    *y = *x ^ *y;
    *x = *x ^ *y;
}


int main(int argc, char* argv[]) {
    int x, y;
    puts("Enter x and y:");
    scanf("%d %d", &x, &y);

    printf("BEFORE SWAP:\n x = %d, y = %d\n", x, y);
    swap_var(&x, &y);
    printf("AFTER SWAP:\n x = %d, y = %d", x, y);

    return 0;
}