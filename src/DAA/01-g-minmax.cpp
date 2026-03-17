/* Algorithm (Pairwise Iterative MinMax):
1. Start
2. Define a `minMax` function:
   a. Take an array as input.
   b. If the array is empty, abort with an error.
   c. Initialize `min` and `max`:
      - If arr.size() is odd:  min = max = arr[0]; start pairing from index 1.
      - If arr.size() is even: compare arr[0] vs arr[1]; smaller -> min, larger
        -> max; start pairing from index 2.
   d. Iterate through remaining elements in pairs (i, i+1):
      i.  Compare arr[i] vs arr[i+1] to find the local smaller and larger.
      ii. If local smaller < min, update min.
      iii.If local larger  > max, update max.
   e. Return {min, max}.
   Total comparisons: floor(3n/2) - 2  (better than naive 2(n-1)).
3. In the `main` function:
   a. Define the array.
   b. Print the original array.
   c. Call `minMax` and print the results.
4. End
*/

#include <iostream>
#include <vector>

#include "src/commons.hpp"

struct MinMaxResult {
    int min;
    int max;
};

MinMaxResult minMax(const std::vector<int>& arr) {
    if (arr.empty()) {
        abortOnError("Array must not be empty.\n");
    }

    int min, max;
    int start;

    // Initialise min/max from the first pair (or single element if odd size)
    if (arr.size() % 2 != 0) {
        min = max = arr[0];
        start = 1;
    }
    else {
        if (arr[0] < arr[1]) {
            min = arr[0];
            max = arr[1];
        }
        else {
            min = arr[1];
            max = arr[0];
        }
        start = 2;
    }

    // Process remaining elements in pairs
    for (int i = start; i < (int)arr.size() - 1; i += 2) {
        int localMin, localMax;

        if (arr[i] < arr[i + 1]) {
            localMin = arr[i];
            localMax = arr[i + 1];
        }
        else {
            localMin = arr[i + 1];
            localMax = arr[i];
        }

        if (localMin < min) min = localMin;
        if (localMax > max) max = localMax;
    }

    return { min, max };
}

int main() {
    Header("MIN-MAX");
    std::vector<int> array = { 3, 5, 1, 8, 2, 9, 4, 7, 6 };

    std::cout << "Array: " << ToString(array) << std::endl;

    MinMaxResult result = minMax(array);
    std::cout << "Minimum: " << result.min << std::endl;
    std::cout << "Maximum: " << result.max << std::endl;

    Footer();
    return 0;
}
