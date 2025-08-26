#define SURAB_OS
#include "src/commons.h"

void FCFS(Metrics *m) {

  // calculate waiting and burst time for all processes
  for (int i = 1; i < m->count; i++) {
    m->waitTime[i] = m->waitTime[i - 1] + m->burstTime[i - 1];
    m->turnAroundTime[i - 1] = m->waitTime[i - 1] + m->burstTime[i - 1];
  }
  m->turnAroundTime[m->count - 1] =
      m->waitTime[m->count - 1] + m->burstTime[m->count - 1];
}

int main() {
  Metrics m = inputMetrics();
  FCFS(&m);
  AvgMetrics avg = calculateAverageMetrics(&m);
  printMetrics(&m, &avg);
  deinit(&m);
  footer();
  return 0;
}
