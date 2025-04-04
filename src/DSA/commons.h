#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

typedef struct {
    int* arr;
    size_t size;
} Array;


Array promptArray() {
    Array array = { NULL, 0 };
    printf("Size of Array: ");
    scanf("%zu", &array.size);

    array.arr = (int*)malloc(array.size * sizeof(int));
    if (array.arr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    printf("Enter %zu elements:\n", array.size);
    for (size_t i = 0; i < array.size; i++) {
        scanf("%d", &array.arr[i]);
    }

    return array;
}

void printArray(const Array* array) {
    printf("{ ");
    for (size_t i = 0; i < array->size - 1; i++) {
        printf("%d, ", array->arr[i]);
    }
    printf("%d }\n", array->arr[array->size - 1]);
}

void footer() {
    printf("\nSurab Parajuli\nSection: A, Roll: 34\n");
    system("pause");
}