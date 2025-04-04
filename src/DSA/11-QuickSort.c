/* Algorithm:
1. Start
2. Define a `partition` function:
   a. Take an array, `low` index, and `high` index as input.
   b. Set the pivot element as the element at `high`.
   c. Initialize `i` to `low - 1`.
   d. For each element from `low` to `high - 1`:
      i. If the current element is less than or equal to the pivot:
         - Increment `i` and swap the element at `i` with the current element.
   e. Swap the pivot element with the element at `i + 1`.
   f. Return `i + 1` as the partition index.
3. Define a `quickSort` function:
   a. Take an array, `low` index, and `high` index as input.
   b. If `low < high`:
      i. Call `partition` to get the partition index.
      ii. Recursively call `quickSort` for the left and right subarrays.
4. In the `main` function:
   a. Use `promptArray` to input the array.
   b. Call the `quickSort` function with the array, `low = 0`, and `high = size - 1`.
   c. Print the sorted array using `printArray`.
   d. Free the allocated memory and call `footer`.
5. End
*/

#include <stdio.h>
#include "commons.h"

// Partition function to rearrange elements around the pivot
int partition(int arr[], int low, int high) {
    // Choose the last element as the pivot
    int pivot = arr[high];
    // Index of the smaller element
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            // Swap arr[i] and arr[j]
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    // Swap the pivot element with the element at i + 1
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    // Return the partition index
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        // Partition index
        int pi = partition(arr, low, high);

        // Recursively sort elements before and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    // From commons.h
    Array array = promptArray();

    quickSort(array.arr, 0, array.size - 1);

    printf("Sorted array:\n");
    // From commons.h
    printArray(&array);

    free(array.arr);
    // From commons.h
    footer();

    return 0;
}