#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int child_pid = fork();
  if (child_pid == 0) {
    printf("[%d] Child Process! PPID = %d\n", getpid(), getppid());
  }

  int child_pid_2 = fork();

  if (child_pid_2 == 0) {
    printf("[%d] Child Process! PPID = %d\n", getpid(), getppid());
  }
  return 0;
}
