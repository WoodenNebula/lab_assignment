/* Algorithm:
1. Start
2. Define a `bubbleSort` function:
   a. Take an array and its size as input.
   b. For each pass (from 0 to size - 2):
      i. Iterate through the array from index 0 to size - pass - 2.
      ii. If the current element is greater than the next element, swap them.
   c. Repeat until the array is sorted.
3. In the `main` function:
   a. Take the size of the array as input.
   b. Input the elements of the array.
   c. Call the `bubbleSort` function with the array and its size.
   d. Print the sorted array.
4. End
*/

#include <stdio.h>
#include "commons.h"

void bubbleSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap arr[j] and arr[j + 1]
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    // From commons.h
    Array array = promptArray();

    bubbleSort(array.arr, array.size);

    printf("Sorted array:\n");

    // From commons.h
    printArray(&array);

    free(array.arr);

    // From commons.h
    footer();
    return 0;
}