/* Find sum of first 100 natural number using loop */

#include <stdio.h>

int main(int argc, char *argv[]) {
    const int TARGET = 100;
    int sum = 0, i = 1;

    while (i <= TARGET)
    {
        sum += i;
        i++;
    }

    printf("Sum = %d", sum);

    return 0;
}