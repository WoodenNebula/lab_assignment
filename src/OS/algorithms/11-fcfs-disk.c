#include <stdio.h>
#include <stdlib.h>

#define SURAB_ARRAY
#include "src/commons.h"

int main() {
  Array requests =
      promptArray("Enter no. of requests:", "Enter request sequence:");

  int head, total = 0;
  printf("Enter initial head position: ");
  scanf("%d", &head);

  for (int i = 0; i < requests.size; i++) {
    total += abs(requests.arr[i] - head);
    head = requests.arr[i];
  }
  printf("Total head movement: %d\n", total);
  printf("Average head movement: %.2f\n", (float)total / requests.size);
  return 0;
}
