/* WAP to print your name. date of birth and mobile number and name of college
 using printf() and puts() functions in following format */

#include <stdio.h>


int main(void) {
    char student_name[] = "Surab Parajuli";
    char dob[] = "2005/11/05";
    char  mobile_num[10 + 1] = "9842069069";
    char college_name[] = "Patan Multiple Campus";


    printf("Name: ");
    puts(student_name);

    printf("DoB(IN A.D): ");
    puts(dob);

    printf("Mobile No: ");
    puts(mobile_num);

    printf("Campus: ");
    puts(college_name);

    return 0;
}