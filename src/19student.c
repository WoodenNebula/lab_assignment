/* Define a structure to store the data for a student with fields (rollno,first_name, last_name, address, mobileno), input the data for n students and display the records in appropriate format */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned int rollno;
    char first_name[20];
    char last_name[20];
    unsigned long long int mobileno; 
} Student;


int main(int argc, char *argv[]) {
    int n = 0;

    while(n < 1) {
        puts("Enter the no of students:");
        scanf("%d", &n);
        
        if(n < 1) {
            printf("\033[1;31m[ERROR] Number of students must be a positive integer!\033[0m\n");
        }
    }

    Student students[n];
    for(int i = 0; i < n; i++) {
        printf("Input student[%d]:\n", i + 1);
        printf("\tRoll No: ");
        scanf("%u", &students[i].rollno);
        printf("\tFirst Name: ");
        scanf("%s", students[i].first_name);
        printf("\tLast Name: ");
        scanf("%s", students[i].last_name);
        printf("\tMobile No: ");
        scanf("%lld", &students[i].mobileno);
        puts("");
    }

    system("cls");

    for(int i = 0; i < n; i++) {
        printf("\nStudent[%d]:\n", i + 1);
        printf("\tRoll No: %u", students[i].rollno);
        printf("\n\tFirst Name: %s", students[i].first_name);
        printf("\n\tLast Name: %s", students[i].last_name);
        printf("\n\tMobile No: %llu\n", students[i].mobileno);
    }

    return 0;
}