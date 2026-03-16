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

#include <iostream>
#include <vector>

#include "src/commons.hpp"

void bubbleSort(std::vector<int>& arr) {
    for (int i = 0; i < arr.size() - 1; i++) {
        for (int j = 0; j < arr.size() - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap arr[j] and arr[j + 1]
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    std::cout << "Sorted Array: " << ToString(arr);
}

int main() {
    Header("BUBBLE SORT");
    std::vector<int> array = {1, 2, 6, 9, 10, 34, 4, 0, 19};

    std::cout << "Original Array: " << ToString(array) << std::endl;
    bubbleSort(array);

    Footer();
    return 0;
}
