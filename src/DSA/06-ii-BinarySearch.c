/* Algorithm:
1. Start
2. Define a `binarySearch` function:
   a. Take a sorted array, its size, and the target element as input.
   b. Initialize two pointers: `low = 0` and `high = size - 1`.
   c. While `low <= high`:
      i. Calculate the middle index: `mid = (low + high) / 2`.
      ii. If the element at `mid` matches the target, return `mid`.
      iii. If the target is smaller than the element at `mid`, set `high = mid - 1`.
      iv. If the target is larger than the element at `mid`, set `low = mid + 1`.
   d. If the target is not found, return -1.
3. In the `main` function:
   a. Take the size of the array as input.
   b. Input the elements of the array (ensure they are sorted).
   c. Take the target element to search for as input.
   d. Call the `binarySearch` function with the array, size, and target.
   e. If the function returns -1, print "Element not found".
   f. Otherwise, print the index of the target element.
4. End
*/

#include <stdio.h>
#include "commons.h"

int binarySearch(int arr[], int size, int target) {
    int low = 0, high = size - 1;

    while (low <= high) {
        int mid = (low + high) / 2;

        if (arr[mid] == target) {
            return mid;
        }
        else if (arr[mid] > target) {
         // Search in the left half
            high = mid - 1;
        }
        else {
         // Search in the right half
            low = mid + 1;
        }
    }

    return -1;
}

int main() {
    // From commons.h
    Array array = {
        .arr = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 },
    };
    array.size = sizeof(array.arr) / sizeof(int);

    //  From commons.h
    printArray(&array);

    int target;
    printf("Search for: ");
    scanf("%d", &target);

    int result;
    result = binarySearch(array.arr, array.size, target);

    if (result == -1) {
        printf("Not Found\n");
    }
    else {
        printf("Found at index %d\n", result);
    }

    free(array.arr);
    footer();
    return 0;
}