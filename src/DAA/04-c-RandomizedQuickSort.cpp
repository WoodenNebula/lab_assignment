/* Algorithm (Randomized Quick Sort - Divide and Conquer):
1. Start
2. Define `lomutoPartition(arr, low, high)`:
   a. Set pivot = arr[high], i = low - 1.
   b. For j from low to high-1:
      - If arr[j] <= pivot: increment i, swap arr[i] and arr[j].
   c. Swap arr[i+1] with arr[high] and return i+1.
3. Define `randomizedPartition(arr, low, high)`:
   a. Pick a random index r in [low, high].
   b. Swap arr[r] with arr[high] to use a random element as pivot.
   c. Return lomutoPartition(arr, low, high).
4. Define `randomizedQuickSort(arr, low, high)`:
   a. If low < high:
      i.  pi = randomizedPartition(arr, low, high).
      ii. randomizedQuickSort(arr, low, pi-1).
      iii.randomizedQuickSort(arr, pi+1, high).
   Randomization avoids worst-case O(n²) on already-sorted inputs.
5. In `main`:
   a. Define the array.
   b. Print original array.
   c. Call randomizedQuickSort and print sorted array.
6. End
*/

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "src/commons.hpp"

int partition(std::vector<int>& arr, int low, int high) {
    int left = low, right = high;
    int pivot = arr[low];

    while (left < right) {
        while (arr[left] <= pivot && left < right) left++;
        while (arr[right] > pivot) right--;

        if (left < right) std::swap(arr[left], arr[right]);
    }

    std::swap(arr[low], arr[right]);
    return right;
}


int randomizedPartition(std::vector<int>& arr, int low, int high) {
    int r = low + rand() % (high - low + 1);
    std::swap(arr[r], arr[low]);
    return partition(arr, low, high);
}

void randomizedQuickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = randomizedPartition(arr, low, high);
        randomizedQuickSort(arr, low, pi - 1);
        randomizedQuickSort(arr, pi + 1, high);
    }
}

int main() {
    srand((unsigned int)time(nullptr));

    Header("RANDOMIZED QUICK SORT");
    std::vector<int> array = { 10, 7, 8, 9, 1, 5 };

    std::cout << "Original Array: " << Surab::ToString(array) << std::endl;
    randomizedQuickSort(array, 0, (int)array.size() - 1);
    std::cout << "Sorted Array:   " << Surab::ToString(array) << std::endl;

    Footer();
    return 0;
}
