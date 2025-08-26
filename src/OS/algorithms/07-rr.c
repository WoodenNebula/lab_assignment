#define SURAB_OS
#define LOG_LEVEL 0
#include <stdio.h>

#include "src/commons.h"

void RR(Metrics *m, int timeQuanta) {
  int totalTime = 0, remainingProcess = m->count;

  char msg[255] = "";
  sprintf(msg, "PROCESS STARTED FOR %d", m->count);
  LOG(msg);

  while (remainingProcess > 0) {
    // calculate waiting and burst time for all processes
    for (int i = 0; i < m->count; i++) {
      if (m->remainingTime[i] > 0) {
        if (m->remainingTime[i] > timeQuanta) {
          // execute for full timeQuanta if remainingTime > timeQuanta
          totalTime += timeQuanta;
          m->remainingTime[i] -= timeQuanta;
        } else {
          // execute for the remaining of the time
          totalTime += m->remainingTime[i];
          m->waitTime[i] = totalTime - m->burstTime[i];
          m->remainingTime[i] = 0;
          // LOG("PROCESS COMPLETE");
          remainingProcess--;
        }
      }
    }
  }

  for (int i = 0; i < m->count; i++) {
    m->turnAroundTime[i] = m->burstTime[i] + m->waitTime[i];
  }

  LOG("PROCESS COMPLETED FOR ALL");
}

int main() {
  int timeQuanta = 0;
  printf("Enter Time Quantam: ");
  scanf("%d", &timeQuanta);

  Metrics m = inputMetrics();
  RR(&m, timeQuanta);
  AvgMetrics avg = calculateAverageMetrics(&m);

  printMetrics(&m, &avg);
  deinit(&m);
  footer();
  return 0;
}
