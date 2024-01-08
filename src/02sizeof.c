#include <stdio.h>

void print_size(const char* type, int size){
    printf("%s = %d\n", type, size);
}

int main(int argc, char *argv[]) {

    printf(("Printing the size of all datatypes[in Bytes]:\n"));

    print_size("int", sizeof(int));
    print_size("char", sizeof(char));
    print_size("float", sizeof(float));
    print_size("double", sizeof(double));
    print_size("long", sizeof(long));
    print_size("long long", sizeof(long long));
    print_size("unsigned", sizeof(unsigned));
    print_size("short", sizeof(short));
    print_size("int*", sizeof(int*));

    return 0;
}