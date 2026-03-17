/* Algorithm (Binary Search - Divide and Conquer):
1. Start
2. Define `binarySearch(arr, low, high, target)`:
   a. If low > high, return -1 (element not found).
   b. Compute mid = low + (high - low) / 2.   (avoids overflow vs (low+high)/2)
   c. If arr[mid] == target, return mid.
   d. If arr[mid] < target, recurse on the right half:
         return binarySearch(arr, mid+1, high, target).
   e. Else recurse on the left half:
         return binarySearch(arr, low, mid-1, target).
3. In `main`:
   a. Define a sorted array and a target to search for.
   b. Call binarySearch and print the index or "Not found".
4. End
*/

#include <iostream>
#include <vector>

#include "src/commons.hpp"

int binarySearch(const std::vector<int>& arr, int low, int high, int target) {
    if (low > high) return -1;

    int mid = low + (high - low) / 2;

    if (arr[mid] == target) return mid;
    if (arr[mid] < target)  return binarySearch(arr, mid + 1, high, target);
    return binarySearch(arr, low, mid - 1, target);
}

int main() {
    Header("BINARY SEARCH");
    std::vector<int> array = { 2, 5, 8, 12, 16, 23, 38, 45, 56, 72, 91 };
    int target = 23;

    std::cout << "Array:  " << ToString(array) << std::endl;
    std::cout << "Target: " << target << std::endl;

    int index = binarySearch(array, 0, (int)array.size() - 1, target);

    if (index != -1)
        std::cout << "Found at index: " << index << std::endl;
    else
        std::cout << "Not found." << std::endl;

    Footer();
    return 0;
}
