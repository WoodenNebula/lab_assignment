#define SURAB_OS
#include "src/commons.h"

AvgMetrics FCFS(Metrics *m) {
  AvgMetrics avg = {0, 0};

  // calculate waiting and burst time for all processes
  for (int i = 1; i < m->count; i++) {
    m->waitTime[i] = m->waitTime[i - 1] + m->burstTime[i - 1];
    avg.waitTime += m->waitTime[i - 1];

    m->turnAroundTime[i - 1] = m->waitTime[i - 1] + m->burstTime[i - 1];
    avg.turnAroundTime += m->turnAroundTime[i - 1];
  }

  avg.waitTime /= m->count;
  avg.turnAroundTime /= m->count;
  return avg;
}

int main() {
  Metrics m = inputMetrics();
  AvgMetrics avg = FCFS(&m);
  printMetrics(&m, &avg);
  deinit(&m);
  footer();
  return 0;
}
