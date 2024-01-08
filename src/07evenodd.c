/* Check a given integer is positive even, negative even, positive odd or negative odd */

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int num;
    puts("Enter a number:");
    scanf("%d", &num);


    if (num == 0){
        printf("\nThe number is 0 (Positive and Even)");
    }
    else
    {
        char pos_neg[10];
        char even_odd[5];

        if (num > 0){
            strcpy(pos_neg, "Positive");
        }
        else if(num < 0){
            strcpy(pos_neg, "Negative");
        }

        if (num % 2 == 0){
            strcpy(even_odd, "Even");
        } 
        else{
            strcpy(even_odd, "Odd");
        }

        printf("\nThe number is %s-%s", pos_neg, even_odd);
    }

    return 0;
}