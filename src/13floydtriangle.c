/* Print Floyd's Triangle till input n
EXAMPLE: N = 6
1
2 3
4 5 6 */

#include <stdio.h>

int main(int argc, char *argv[]) {
    int n;
    puts("Enter n-rows for the Floyd's Triangle:");
    scanf("%d", &n);

    for(int num = 1, line = 1; num < n;) {
        for (int i = line; i > 0; i--) {
            printf("%d ", num);
            num++;
        }
        line++;
        puts("");
    }

    return 0;
}