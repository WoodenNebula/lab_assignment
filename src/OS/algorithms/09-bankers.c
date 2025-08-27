#define SURAB_OS
#define SURAB_MATRIX
#define LOG_LEVEL 0
#include <stdio.h>

#include "src/commons.h"

int main() {
  int processesCount, resourceCount;
  printf("Enter number of processes: ");
  scanf("%d", &processesCount);
  printf("Enter number of resources: ");
  scanf("%d", &resourceCount);

  Matrix alloc = {processesCount, resourceCount};
  inputMatrix(&alloc,
              "Enter Allocation Matrix:[processesCount X resourceCount]");

  Matrix max = {processesCount, resourceCount};
  inputMatrix(&max, "Enter Max Matrix:[processesCount X resourceCount]");

  int avail[resourceCount];
  printf("\nEnter Available Resources:\n");
  for (int j = 0; j < resourceCount; j++) {
    scanf("%d", &avail[j]);
  }
  // Calculate Need matrix
  Matrix need = {processesCount, resourceCount};
  for (int i = 0; i < processesCount; i++) {
    for (int j = 0; j < resourceCount; j++) {
      need.data[i][j] = max.data[i][j] - alloc.data[i][j];
    }
  }

  int finish[processesCount];
  // Initialize finish array
  for (int i = 0; i < processesCount; i++) {
    finish[i] = 0;
  }

  int safeSeq[processesCount];
  int count = 0;

  while (count < processesCount) {
    int found = 0;
    for (int p = 0; p < processesCount; p++) {
      if (finish[p] == 0) {
        int canAllocate = 1;
        for (int j = 0; j < resourceCount; j++) {
          if (need.data[p][j] > avail[j]) {
            canAllocate = 0;
            break;
          }
        }
        if (canAllocate) {
          for (int j = 0; j < resourceCount; j++)
            avail[j] += alloc.data[p][j];
          safeSeq[count++] = p;
          finish[p] = 1;
          found = 1;
        }
      }
    }
    if (!found) {
      printf("\n DEADLOCK detected! No Safe Sequence avaliable\n");
      footer();
      return 0;
    }
  }

  printf("\nSAFE STATE.\nSafe sequence is: ");
  for (int i = 0; i < processesCount - 1; i++) {
    printf("P[%d]->", safeSeq[i] + 1);
  }
  printf("P[%d]\n", safeSeq[processesCount - 1] + 1);

  footer();
  return 0;
}
