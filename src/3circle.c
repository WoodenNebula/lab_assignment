/* Write algorithm, flowchart and program to compute the area and circumference of a circle with given radus r as input defining PI as constant*/
#include <stdio.h>


///TODO: Write ALGORITHM and make FLOWCHART
int main(int argc, char *argv[]) {
    const float PI = 3.14;
    float radius, area, circum;

    puts("Enter the radius of circle.");
    scanf("%f", &radius);

    area = PI * radius * radius;
    circum = 2 * PI * radius;

    printf("Area = %.2f, Circumference = %.2f", area, circum);

    return 0;
}