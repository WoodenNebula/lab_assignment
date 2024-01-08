/* Write a program to convert specified no. of days into years, weeks and days ignoring leap years */
#include <stdio.h>

int main(int argc, char *argv[]) {
    int numDays;
    puts("Enter numer of days:");
    scanf("%d", &numDays);

    int years = 0, weeks = 0, days = 0;

    years = numDays / 365;
    numDays %= 365;
    weeks = numDays / 7;
    numDays %= 7;
    days = numDays;

    printf("\n%d Year(s), %d Week(s), %d Day(s)", years, weeks, days);

    return 0;
}
