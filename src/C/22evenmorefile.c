/* Read a text file and copy all contents in to a new file */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    const char src_path[] = "src\\animal.txt";
    const char dest_path[] = "src\\animal-copy.txt";
    FILE *src = fopen(src_path, "r");
    if(!src) {
        printf("\033[1;31mERROR! FILE DIDN'T OPEN\033[0m");
        exit(EXIT_FAILURE);
    }

    FILE *dest = fopen(dest_path, "w");
    if(!dest) {
        printf("\033[1;31mERROR! FILE DIDN'T OPEN\033[0m");
        exit(EXIT_FAILURE);
    }

    char line[32];
    printf("Copying File...\n");
    while ( fgets(line, 31, src) != NULL) {
        fprintf(dest, line);
    }
    printf("File Copied!->%s\n", dest_path);
    fclose(src);
    fclose(dest);


    dest = fopen(dest_path, "r");
    
    puts("\nDISPLAYING COPIED FILE CONTENT:");
    while( fscanf(dest, "%s", line) != EOF ) {
        printf("%s\n", line);
    }
    
    fclose(dest);

    return 0;
}