/* Algorithm:
1. Start
2. Define a `merge` function:
   a. Take an array, `left` index, `mid` index, and `right` index as input.
   b. Create temporary arrays for the left and right subarrays.
   c. Copy elements from the original array into the temporary arrays.
   d. Merge the two subarrays back into the original array in sorted order.
3. Define a `mergeSort` function:
   a. Take an array, `left` index, and `right` index as input.
   b. If `left < right`:
      i. Calculate the middle index: `mid = (left + right) / 2`.
      ii. Recursively call `mergeSort` for the left and right subarrays.
      iii. Call `merge` to merge the two sorted subarrays.
4. In the `main` function:
   a. Take the size of the array as input.
   b. Input the elements of the array.
   c. Call the `mergeSort` function with the array, `left = 0`, and `right = size - 1`.
   d. Print the sorted array.
5. End
*/

#include <stdio.h>
#include "commons.h"

// Merge function to combine two sorted subarrays
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1; // Size of the left subarray
    int n2 = right - mid;    // Size of the right subarray

    // Create temporary arrays
    int leftArr[n1], rightArr[n2];

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++) {
        leftArr[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        rightArr[j] = arr[mid + 1 + j];
    }

    // Merge the temporary arrays back into the original array
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
        }
        else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // Copy any remaining elements of leftArr
    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // Copy any remaining elements of rightArr
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

// MergeSort function
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2; // Calculate the middle index

        // Recursively sort the left and right subarrays
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Merge the sorted subarrays
        merge(arr, left, mid, right);
    }
}

int main() {
    Array array = promptArray();

    mergeSort(array.arr, 0, array.size - 1);

    printf("Sorted array:\n");
    printArray(&array);

    free(array.arr);

    printf("\nSurab Parajuli\nSection: A, Roll: 34\n");
    system("pause");
    return 0;
}