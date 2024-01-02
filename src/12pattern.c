/* Pattern of * using loop upto n lines */

#include <stdio.h>

int main(int argc, char *argv[]) {
    int n;
    puts("Enter no. of lines of pattern to be displayed:");
    scanf("%d", &n);

    // Left Aligned
    for (int i = 0; i < n; i++) {
        for(int j = 0; j <= i; j++) {
            printf("* ");
        }
        puts("");
    }

    puts("");
    puts("");

    // Mid Aligned
    for (int i = 0; i < n; i++) {
        for(int j = 0; j < n - i; j++)
            printf(" ");
        
        for(int k = 0; k <= i; k++) {
            printf("* ");
        }
        puts("");
    }

    puts("");
    puts("");

    // Right Aligned
    for (int i = 0; i < n; i++) {
        for(int j = 0; j < n - i; j++)
            printf("  ");
        
        for(int k = 0; k <= i; k++) {
            printf("* ");
        }
        puts("");
    }

    return 0;
}