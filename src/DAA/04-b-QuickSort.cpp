/* Algorithm (Quick Sort - Divide and Conquer):
1. Start
2. Define `partition(arr, low, high)`:
   a. Set pivot = arr[high], i = low - 1.
   b. For j from low to high-1:
      - If arr[j] <= pivot: increment i, swap arr[i] and arr[j].
   c. Swap arr[i+1] with arr[high] (place pivot in its correct position).
   d. Return i+1 as the pivot's final index.
3. Define `quickSort(arr, low, high)`:
   a. If low < high:
      i.  pi = partition(arr, low, high).
      ii. quickSort(arr, low, pi-1).   // Conquer: sort left sub-array
      iii.quickSort(arr, pi+1, high).  // Conquer: sort right sub-array
4. In `main`:
   a. Define the array.
   b. Print original array.
   c. Call quickSort and print sorted array.
5. End
*/

#include <iostream>
#include <vector>

#include "src/commons.hpp"

int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i     = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }

    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    Header("QUICK SORT");
    std::vector<int> array = { 10, 7, 8, 9, 1, 5 };

    std::cout << "Original Array: " << ToString(array) << std::endl;
    quickSort(array, 0, (int)array.size() - 1);
    std::cout << "Sorted Array:   " << ToString(array) << std::endl;

    Footer();
    return 0;
}
