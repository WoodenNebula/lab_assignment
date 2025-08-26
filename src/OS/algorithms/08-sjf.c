#define SURAB_OS
#define LOG_LEVEL 0
#include <stdio.h>

#include "src/commons.h"

void SJF(Processes *p, int timeQuanta) {
  int totalTime = 0, remainingProcess = p->count;

  char msg[255] = "";
  sprintf(msg, "PROCESS STARTED FOR %d", p->count);
  LOG(msg);

  while (remainingProcess > 0) {
    // calculate waiting and burst time for all processes
    for (int i = 0; i < p->count; i++) {
      if (p->m[i].remainingTime > 0) {
        if (p->m[i].remainingTime > timeQuanta) {
          // execute for full timeQuanta if remainingTime > timeQuanta
          totalTime += timeQuanta;
          p->m[i].remainingTime -= timeQuanta;
        } else {
          // execute for the remaining of the time
          totalTime += p->m[i].remainingTime;
          p->m[i].waitTime = totalTime - p->m[i].burstTime;
          p->m[i].remainingTime = 0;
          // LOG("PROCESS COMPLETE");
          remainingProcess--;
        }
      }
    }
  }

  for (int i = 0; i < p->count; i++) {
    p->m[i].turnAroundTime = p->m[i].burstTime + p->m[i].waitTime;
  }

  LOG("PROCESS COMPLETED FOR ALL");
}

void SortByBT(Processes *p) {
  for (int i = 0; i < p->count; i++) {
    for (int j = i + 1; j < p->count; j++) {
      if (p->m[j].burstTime < p->m[i].burstTime) {
        printf("Before swap: (b, w, ta, r) = (%d, %d, %d, %d)",
               p->m[i].burstTime, p->m[i].waitTime, p->m[i].turnAroundTime,
               p->m[i].remainingTime);
        // swap
        Metrics tmp = p->m[j];
        p->m[j] = p->m[i];
        p->m[i] = tmp;

        printf("After swap: (b, w, ta, r) = (%d, %d, %d, %d)",
               p->m[i].burstTime, p->m[i].waitTime, p->m[i].turnAroundTime,
               p->m[i].remainingTime);
      }
    }
  }
}

int main() {
  int timeQuanta = 0;
  printf("Enter Time Quantam: ");
  scanf("%d", &timeQuanta);

  Processes p = inputMetrics();

  SortByBT(&p);
  SJF(&p, timeQuanta);
  AvgMetrics avg = calculateAverageMetrics(&p);

  printProcesses(&p, &avg);
  deinit(&p);
  footer();
  return 0;
}
