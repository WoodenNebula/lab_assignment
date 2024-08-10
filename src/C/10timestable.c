/* Display multiplication of 1 to n where n is input number */

#include <stdio.h>

int main(int argc, char *argv[]) {
    const int TABLE_OF = 1;
    int n;
    puts("Enter n till which the table is to be displayed:");
    scanf("%d", &n);

    for(int i = 1; i <= n; i++){
        printf("%d X %d = %d\n", TABLE_OF, i, TABLE_OF * i);
    }

    return 0;
}