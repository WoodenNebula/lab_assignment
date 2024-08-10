/* Prompt user to input filename and read the content of file and display it in screen */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ERROR(msg) {printf("\033[1;31m[ERROR] line %d: %s\033[0m\n", __LINE__, msg); exit(EXIT_FAILURE);}

#define MAX_FILE_OPTN 3

static int g_line_num;

int verify_input(char usr_choice[], char lines[][50]) {
    int i = 0;
    while (i < MAX_FILE_OPTN) {
        if( strncasecmp(usr_choice, &(lines[i][3]), strlen(usr_choice)) == 0) {
            g_line_num = i;
            return 1;
        }
        i++;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    char lines[MAX_FILE_OPTN][50];
    {   // Initial Menu
        FILE* file_list = fopen("src\\file-list.txt", "r");
        int line_num = 0;
        
        if(!file_list) {
            ERROR("FILE DIDN'T OPEN!");
        }

        puts("Chose a file name to open:");

        while( fgets(lines[line_num], sizeof(lines[line_num]), file_list) != NULL ) {
            printf("%s", lines[line_num]);
            line_num++;
        }
        puts("\n");

        fclose(file_list);
    }   // Initial Menu
 
    char usr_choice[25];
    {   // Input
        scanf("%s", usr_choice);
        while ( verify_input(usr_choice, lines) != 1 ) {
            printf("THAT FILE DOESN'T EXIST! TRY AGAIN!\n");
            scanf("%s", usr_choice);
        }

    }   //--INPUT

    {   // opening and displaying the file content of usr_choice
        char file_path[40];
        system("cls");
        printf("\033[1;32mFile %s:\033[0m\n\n", usr_choice);
        strlwr(usr_choice);
        sprintf(file_path, "src\\%s.txt", usr_choice);

        FILE *file = fopen(file_path, "r");
        if(!file) {
            ERROR("FILE DIDN'T OPEN!");
        }

        char line_buffer[50];
        while( fgets(line_buffer, sizeof(line_buffer), file) != NULL ) {
            printf("%s", line_buffer);
        }
        puts("\n");

        fclose(file);


    }
    return 0;
}