/* Input string, show its length and reverse it b) using own function*/

#include <stdio.h>

int GetStringLen(char* string){
    int i = 0;

    while(string[i] != '\0')
    {
        i++;
    }

    return i;
}


void ReverseString(char* source){
    int string_len = GetStringLen(source);
    char temp_buffer;
    for(int start = 0, end = string_len - 1; start < end; start++, end--)
    {
        temp_buffer = source[start];
        source[start] = source[end];
        source[end] = temp_buffer;
    }
}


int main(int argc, char *argv[]) {
    char in_string[36];

    puts("Enter string input:");
    scanf("%[^\n]s", in_string);

    int string_len = GetStringLen(in_string);


    ReverseString(in_string);

    printf("String length = %d\n", string_len);
    printf("Reversed String = %s", in_string);


    return 0;
}