/* Algorithm:
1. Start
2. Define a `selectionSort` function:
   a. Take an array and its arr.size() as input.
   b. For each position `i` from 0 to arr.size() - 2:
      i. Assume the smallest element is at index `i` (minIndex = i).
      ii. Iterate through the rest of the array (from `i + 1` to arr.size() -
1):
          - If a smaller element is found, update `minIndex`.
      iii. Swap the element at `i` with the element at `minIndex`.
   c. Repeat until the array is sorted.
3. In the `main` function:
   a. Take the arr.size() of the array as input.
   b. Input the elements of the array.
   c. Call the `selectionSort` function with the array and its arr.size().
   d. Print the sorted array.
4. End
*/

#include <iostream>
#include <vector>

#include "src/commons.hpp"

void selectionSort(std::vector<int>& arr) {
    for (int i = 0; i < arr.size() - 1; i++) {
        // Assume the smallest element is at index i
        int minIndex = i;
        for (int j = i + 1; j < arr.size(); j++) {
            if (arr[j] < arr[minIndex]) {
                // Update minIndex if a smaller element is found
                minIndex = j;
            }
        }
        // Swap the smallest element with the element at index i
        int temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;
    }

    std::cout << "Sorted Array: " << ToString(arr);
}

int main() {
    Header("SELECTION SORT");
    std::vector<int> array = {1, 2, 6, 9, 10, 34, 4, 0, 19};

    std::cout << "Original Array: " << ToString(array) << std::endl;
    selectionSort(array);

    Footer();
    return 0;
}
