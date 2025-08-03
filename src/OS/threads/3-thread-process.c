#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void *show(void *u) {
  int pid;
  printf("\n[%d]->[%d] : Old Thread ", getppid(), getpid());
}

int main() {
  int pid;
  pthread_t tid;
  pthread_t tid_child;
  time_t t;
  time(&t);
  printf("\nThis program has been written at (date and time): %s", ctime(&t));
  pthread_create(&tid, NULL, &show, NULL);
  printf("\n[%d]->[%d] : Main thread", getppid(), getpid());

  pid = fork();
  if (pid == 0) {
    printf("\n[%d]->[%d] : Child Process", getppid(), getpid());
    exit(0);
  }

  pthread_join(tid, NULL);
  return 0;
}
