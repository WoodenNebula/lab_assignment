#include <stdio.h>

#define SURAB_ARRAY
#include "src/commons.h"

int main() {
  Array blocks = promptArray("Enter number of memory blocks:",
                             "Enter size of each block:");
  Array blockState = {NULL, blocks.size};
  array_init(&blockState);
  Array processes = promptArray("Enter number of processes:",
                                "Enter size of each processes:");

  Array allocation = {NULL, processes.size};
  array_init(&allocation);
  for (int i = 0; i < allocation.size; i++) {
    allocation.arr[i] = -1;
  }

  // First Fit allocation
  for (int i = 0; i < processes.size; i++) {
    for (int j = 0; j < blocks.size; j++) {
      if (blockState.arr[j]) {
        // block is already allocated, skip it
        continue;
      }
      if (blocks.arr[j] >= processes.arr[i]) {
        allocation.arr[i] = j;
        blockState.arr[j] = 1;
        blocks.arr[j] -= processes.arr[i];
        break;
      }
    }
  }

  printf("\nProcess No.\tProcess Size\tBlock No.\n");
  for (int i = 0; i < processes.size; i++) {
    printf("%d\t\t%d\t\t", i + 1, processes.arr[i]);
    if (allocation.arr[i] != -1)
      printf("%d\n", allocation.arr[i] + 1);
    else
      printf("Not Allocated\n");
  }

  array_deinit(&blocks);
  array_deinit(&blockState);
  array_deinit(&processes);
  array_deinit(&allocation);
  footer();
  return 0;
}
