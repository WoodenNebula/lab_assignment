/* Algorithm (MinMax - Divide and Conquer):
1. Start
2. Define `minMax(arr, low, high)`:
   a. Base case 1 — single element (low == high):
      Return {arr[low], arr[low]}.
   b. Base case 2 — two elements (high == low + 1):
      Return {min(arr[low], arr[high]), max(arr[low], arr[high])}.
   c. Divide: mid = (low + high) / 2.
   d. Conquer:
      leftResult  = minMax(arr, low,    mid).
      rightResult = minMax(arr, mid+1,  high).
   e. Combine:
      Return { min(leftResult.min, rightResult.min),
               max(leftResult.max, rightResult.max) }.
3. In `main`:
   a. Define the array.
   b. Call minMax and print min and max.
4. End
*/

#include <iostream>
#include <vector>

#include "src/commons.hpp"

struct MinMaxResult {
    int min;
    int max;
};

MinMaxResult minMax(const std::vector<int>& arr, int low, int high) {
    // Base case: single element
    if (low == high) return { arr[low], arr[low] };

    // Base case: two elements
    if (high == low + 1) {
        if (arr[low] < arr[high]) return { arr[low], arr[high] };
        return { arr[high], arr[low] };
    }

    int mid = (low + high) / 2;
    MinMaxResult left  = minMax(arr, low, mid);
    MinMaxResult right = minMax(arr, mid + 1, high);

    return {
        left.min < right.min ? left.min : right.min,
        left.max > right.max ? left.max : right.max
    };
}

int main() {
    Header("MIN-MAX (DIVIDE & CONQUER)");
    std::vector<int> array = { 3, 5, 1, 8, 2, 9, 4, 7, 6 };

    std::cout << "Array:   " << ToString(array) << std::endl;

    MinMaxResult result = minMax(array, 0, (int)array.size() - 1);
    std::cout << "Minimum: " << result.min << std::endl;
    std::cout << "Maximum: " << result.max << std::endl;

    Footer();
    return 0;
}
