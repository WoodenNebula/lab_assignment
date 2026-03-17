/* Algorithm (Heap Sort - Divide and Conquer):
1. Start
2. Define `heapify(arr, n, i)`:
   a. Set largest = i, left = 2i+1, right = 2i+2.
   b. If left  < n and arr[left]  > arr[largest], set largest = left.
   c. If right < n and arr[right] > arr[largest], set largest = right.
   d. If largest != i:
      - Swap arr[i] with arr[largest].
      - Recursively heapify the subtree rooted at largest.
3. Define `heapSort(arr)`:
   a. Build max-heap by calling heapify for each non-leaf node
      (i from n/2-1 down to 0).
   b. For i from n-1 down to 1:
      i.  Swap arr[0] (current max) with arr[i] (last element).
      ii. Reduce the heap size by 1.
      iii.Call heapify(arr, i, 0) to restore heap property.
4. In `main`:
   a. Define the array.
   b. Print original array.
   c. Call heapSort and print sorted array.
5. End
*/

#include <iostream>
#include <vector>

#include "src/commons.hpp"

void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int left    = 2 * i + 1;
    int right   = 2 * i + 2;

    if (left  < n && arr[left]  > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(std::vector<int>& arr) {
    int n = (int)arr.size();

    // Build max-heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Extract elements from heap one by one
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

int main() {
    Header("HEAP SORT");
    std::vector<int> array = { 12, 11, 13, 5, 6, 7 };

    std::cout << "Original Array: " << ToString(array) << std::endl;
    heapSort(array);
    std::cout << "Sorted Array:   " << ToString(array) << std::endl;

    Footer();
    return 0;
}
