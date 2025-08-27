#define SURAB_OS
#include "src/commons.h"

void FCFS(Processes *p) {
  // calculate waiting time
  for (int i = 1; i < p->count; i++) {
    p->m[i].waitTime = p->m[i - 1].waitTime + p->m[i - 1].burstTime;
  }

  // and turn around time for all processes
  for (int i = 0; i < p->count; i++) {
    p->m[i].turnAroundTime = p->m[i].waitTime + p->m[i].burstTime;
  }
}

int main() {
  Processes p = inputMetrics();

  FCFS(&p);
  AvgMetrics avg = calculateAverageMetrics(&p);

  printProcesses(&p, &avg);
  processes_deinit(&p);
  footer();
  return 0;
}
