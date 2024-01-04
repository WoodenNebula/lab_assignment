/* Input string, show its length and reverse it a) using string libbrary functions*/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char in_string[36];

    puts("Enter string input:");
    scanf("%[^\n]s", in_string);

    int string_len = strlen(in_string);

    printf("Original String = %s\n", in_string);

    char rev_string[string_len];
    strcpy(rev_string, strrev(in_string));

    printf("String length = %d\n", string_len);
    printf("Reversed String = %s\n", rev_string);



    return 0;
}