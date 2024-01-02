/* Read the score of a student and print the grade according to score as:
    Score >= 80 "Distinction"
    70 <= Score < 80 "First Division"
    55 <= Score < 70 "Second Division"
    40 <= Score < 55 "Third Division"
    Score < 40 "Fail"
*/

#include <stdio.h>

#define Distinction(score) score >= 80
#define First_Division(score) 70 <= score && score < 80
#define Second_Division(score) 55 <= score && score < 70
#define Third_Division(score) 40 <= score && score < 55
#define Fail(score) score < 40

int main(int argc, char *argv[]) {
    int score;
    puts("Enter the score:");
    scanf("%d", &score);

    if(Distinction(score)) 
        puts("Distinction");
    else if(First_Division(score)) 
        puts("First Division");
    else if(Second_Division(score))
        puts("Second Division");
    else if(Third_Division(score)) 
        puts("Third Division");
    else if(Fail(score)) 
        puts("Fail");

    return 0;
}