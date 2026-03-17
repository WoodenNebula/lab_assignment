/* Algorithm (Merge Sort - Divide and Conquer):
1. Start
2. Define `merge(arr, low, mid, high)`:
   a. Copy arr[low..mid]    into temporary array leftArr.
   b. Copy arr[mid+1..high] into temporary array rightArr.
   c. Merge: compare front elements of leftArr and rightArr;
      write the smaller one back into arr, advancing the respective pointer.
   d. Copy any remaining elements from either half.
3. Define `mergeSort(arr, low, high)`:
   a. If low < high:
      i.  mid = (low + high) / 2.
      ii. mergeSort(arr, low, mid).          // Conquer: sort left half
      iii.mergeSort(arr, mid+1, high).       // Conquer: sort right half
      iv. merge(arr, low, mid, high).        // Combine sorted halves
4. In `main`:
   a. Define the array.
   b. Print original array.
   c. Call mergeSort and print sorted array.
5. End
*/

#include <iostream>
#include <vector>

#include "src/commons.hpp"

void merge(std::vector<int>& arr, int low, int mid, int high) {
    std::vector<int> left(arr.begin() + low,    arr.begin() + mid + 1);
    std::vector<int> right(arr.begin() + mid + 1, arr.begin() + high + 1);

    int i = 0, j = 0, k = low;

    while (i < (int)left.size() && j < (int)right.size()) {
        if (left[i] <= right[j])
            arr[k++] = left[i++];
        else
            arr[k++] = right[j++];
    }

    while (i < (int)left.size())  arr[k++] = left[i++];
    while (j < (int)right.size()) arr[k++] = right[j++];
}

void mergeSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        mergeSort(arr, low, mid);
        mergeSort(arr, mid + 1, high);
        merge(arr, low, mid, high);
    }
}

int main() {
    Header("MERGE SORT");
    std::vector<int> array = { 38, 27, 43, 3, 9, 82, 10 };

    std::cout << "Original Array: " << ToString(array) << std::endl;
    mergeSort(array, 0, (int)array.size() - 1);
    std::cout << "Sorted Array:   " << ToString(array) << std::endl;

    Footer();
    return 0;
}
