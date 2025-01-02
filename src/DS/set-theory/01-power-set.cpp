#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

void printPowerSet(vector<char>& set) {
  unsigned int powSetSize = pow(2, set.size());

  for (int counter = 0; counter < powSetSize; counter++) {
    for (int j = 0; j < set.size(); j++) {
        // Check if jth bit in the counter is set
        // If set then print jth element from set
      if (counter & (1 << j))
        cout << set[j];
    }
    cout << endl;
  }
}

int main() {
  vector<char> set = { 'a', 'b', 'c' };
  printPowerSet(set);

  std::cout << "\nSurab Parajuli    Section: A    Roll: 34" << std::endl;
  return 0;
}