/* Read from a text file and count the number of lines and characters in that file */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char file_path[] = "src\\file-list.txt";
    FILE* file = fopen(file_path, "r");

    if(!file) {
        printf("\033[1;31mERROR! FILE DIDN'T OPEN\033[0m");
        exit(EXIT_FAILURE);
    }

    int num_line = 0;
    int num_char = 0;
    char ch;

    while ( (ch = fgetc(file)) != EOF ) {
        if ( ch == ' ') {
            continue;
        } else if( ch != '\n') {
            putchar(ch);
            num_char++;
        } else if( ch == '\n' ) {
            num_line++;
        }
    }
    num_line++;

    printf("\nLines = %d\nCharacters[EXCLUDING WHITESPACES] = %d", num_line, num_char);


    return 0;
}