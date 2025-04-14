/* Algorithm:
1. Start
2. Define an `insertionSort` function:
   a. Take an array and its size as input.
   b. For each position `i` from 1 to size - 1:
      i. Store the current element in a variable `key`.
      ii. Initialize `j` to `i - 1`.
      iii. While `j >= 0` and the element at `j` is greater than `key`:
          - Shift the element at `j` to position `j + 1`.
          - Decrement `j`.
      iv. Place `key` at position `j + 1`.
   c. Repeat until the array is sorted.
3. In the `main` function:
   a. Use `promptArray` to input the array.
   b. Call the `insertionSort` function with the array and its size.
   c. Print the sorted array using `printArray`.
   d. Free the allocated memory and call `footer`.
4. End
*/

#include <stdio.h>
#include "commons.h"

void insertionSort(int arr[], int size) {
    for (int i = 1; i < size; i++) {
        int key = arr[i]; // Store the current element
        int j = i - 1;

        // Shift elements of the sorted portion that are greater than `key`
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }

        // Place `key` in its correct position
        arr[j + 1] = key;

        Array array = { arr, size };
        printArray(&array);
    }
}

int main() {
    // From commons.h
    Array array = promptArray();

    insertionSort(array.arr, array.size);

    printf("Sorted array:\n");
    // From commons.h
    printArray(&array);

    free(array.arr);
    // From commons.h
    footer();

    return 0;
}