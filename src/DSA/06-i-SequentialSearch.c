/* Algorithm:
1. Start
2. Define a `linearSearch` function:
   a. Take an array, its size, and the target element as input.
   b. Iterate through the array from index 0 to size - 1:
      i. If the current element matches the target, return the index.
   c. If the target is not found, return -1.
3. In the `main` function:
   a. Take the size of the array as input.
   b. Input the elements of the array.
   c. Take the target element to search for as input.
   d. Call the `linearSearch` function with the array, size, and target.
   e. If the function returns -1, print "Element not found".
   f. Otherwise, print the index of the target element.
4. End
*/

#include <stdio.h>
#include "commons.h"

int linearSearch(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

int main() {
    // From commons.h
    Array array = {
        .arr = { 10, 20, 30, 40, 50 }
    };
    array.size = sizeof(array.arr) / sizeof(int);

    //  From commons.h
    printArray(&array);

    int target;
    printf("\nSearch for: ");
    scanf("%d", &target);

    int result = linearSearch(array.arr, array.size, target);

    if (result == -1) {
        printf("Not found\n");
    }
    else {
        printf("Found at index %d\n", result);
    }

    free(array.arr);
    footer();
    return 0;
}