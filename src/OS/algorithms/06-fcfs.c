#include <stdio.h>
#include <stdlib.h>

#define LOG_LEVEL 0

#if (LOG_LEVEL == 1)
#define LOG(x) printf("\n[LOG] : %s\n", x)
#else
#define LOG(x)
#endif

typedef struct {
  int count;
  int *burstTime;
  int *waitTime;
  int *turnAroundTime;
} Metrics;

typedef struct {
  float waitTime;
  float turnAroundTime;
} AvgMetrics;

Metrics init(int processCount) {
  Metrics m;
  m.count = processCount;
  m.burstTime = (int *)malloc(sizeof(int) * processCount);
  m.waitTime = (int *)malloc(sizeof(int) * processCount);
  m.turnAroundTime = (int *)malloc(sizeof(int) * processCount);

  LOG("Initialized Metric struct");
  return m;
}

void deinit(Metrics *m) {
  if (m->count <= 0) {
    printf("ERROR: Metric not initialized");
    return;
  }

  m->count = -1;
  free(m->burstTime);
  free(m->waitTime);
  free(m->turnAroundTime);

  LOG("Cleaned up Metric struct");
}

Metrics inputMetrics() {
  int n;
  printf("Enter number of processes: ");
  scanf("%d", &n);

  Metrics m = init(n);

  printf("Enter burst time for each process:\n");
  for (int i = 0; i < n; i++) {
    printf("P[%d]: ", i + 1);
    scanf("%d", m.burstTime + i);
  }

  // initialize first waiting time to be zero
  m.waitTime[0] = 0;
  return m;
}

AvgMetrics processMetrics(Metrics *m) {
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

void printMetrics(const Metrics *m, const AvgMetrics *avg) {
  // formatted output
  printf("\nProcess\tBT\tWT\tTAT\n");
  for (int i = 0; i < m->count; i++) {
    printf("P%d\t%d\t%d\t%d\n", i + 1, m->burstTime[i], m->waitTime[i],
           m->turnAroundTime[i]);
  }
  printf("Average Waiting Time = %f, Average Turn Around Time = %f",
         avg->waitTime, avg->turnAroundTime);
}

int main() {
  int n, i;
  Metrics m = inputMetrics();
  AvgMetrics avg = processMetrics(&m);
  printMetrics(&m, &avg);
  deinit(&m);
  return 0;
}
