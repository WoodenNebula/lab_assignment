#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int g_time = 1;
void *sleep_a(void *u) {
  printf("\nNew Thread");
  g_time += 5;
  printf("\nSleeping for %d seconds", g_time);
  sleep(g_time);
  printf("\nSlept for %d seconds", g_time);
}

void *sleep_b(void *u) {
  printf("\nOld Thread");
  g_time += 2;
  printf("\nSleeping for %d seconds", g_time);
  sleep(g_time);
  printf("\nSlept for %d seconds", g_time);
}

int main() {
  pthread_t tid, tid2;
  time_t t;
  time(&t);
  printf("\nThis program has been written at (date and time): %s", ctime(&t));
  pthread_create(&tid, NULL, &sleep_a, NULL);
  pthread_create(&tid2, NULL, &sleep_b, NULL);
  printf("\nMain thread\n");

  pthread_join(tid, NULL);
  pthread_join(tid2, NULL);
}
