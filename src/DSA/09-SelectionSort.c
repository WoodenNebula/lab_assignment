/* Algorithm:
1. Start
2. Define a `selectionSort` function:
   a. Take an array and its size as input.
   b. For each position `i` from 0 to size - 2:
      i. Assume the smallest element is at index `i` (minIndex = i).
      ii. Iterate through the rest of the array (from `i + 1` to size - 1):
          - If a smaller element is found, update `minIndex`.
      iii. Swap the element at `i` with the element at `minIndex`.
   c. Repeat until the array is sorted.
3. In the `main` function:
   a. Take the size of the array as input.
   b. Input the elements of the array.
   c. Call the `selectionSort` function with the array and its size.
   d. Print the sorted array.
4. End
*/

#include <stdio.h>
#include "commons.h"

void selectionSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        // Assume the smallest element is at index i
        int minIndex = i;
        for (int j = i + 1; j < size; j++) {
            if (arr[j] < arr[minIndex]) {
                // Update minIndex if a smaller element is found
                minIndex = j;
            }
        }
    // Swap the smallest element with the element at index i
        int temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;

        Array array = { arr, size };
        printArray(&array);
    }
}

int main() {
    Array array = promptArray();

    selectionSort(array.arr, array.size);

    printf("Sorted array:\n");
    printArray(&array);

    free(array.arr);
    footer();
    return 0;
}