#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int global;

void square() { global = global * global; }
void cube() { global = global * global * global; }

void *calculate(void *choice) {
  int *id_ptr, taskid;
  sleep(1);
  id_ptr = (int *)choice;
  taskid = *id_ptr;
  printf("\nEnter a number: ");
  scanf("%d", &global);
  if (taskid == 0) {
    square();
    printf("\nthe square is %d: \n", global);
  } else if (taskid == 1) {
    cube();
    printf("\nthe cube is %d: \n", global);
  }
  pthread_exit(NULL);
}
int main() {
  pthread_t threads;
  int *choice = malloc(sizeof(int *));
  int rc;
  time_t t;
  time(&t);
  printf("\nThis program has been writen at (date and time): %s", ctime(&t));
  printf("\nEnter 0 for square and 1 for cube number: ");
  scanf("%d", choice);

  printf("Creating thread \n");
  rc = pthread_create(&threads, NULL, calculate, (void *)choice);
  if (rc) {
    printf("ERROR; return code from pthread_create() is %d\n", rc);
    exit(-1);
  }
  pthread_exit(NULL);
}
