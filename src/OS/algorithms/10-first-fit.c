#include <stdio.h>

#define SURAB_ARRAY
#include "src/commons.h"

int main() {
  Array blocks = promptArray("Enter number of memory blocks:",
                             "Enter size of each block:");
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
      if (blocks.arr[j] >= processes.arr[i]) {
        allocation.arr[i] = j;
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
  array_deinit(&processes);
  array_deinit(&allocation);
  footer();
  return 0;
}
