#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  const char path[] = "/home/noxtar/Dev/lab_assignment/src/OS/outputs/1-print";
  char *args[] = {"src/OS/outputs/1-print", "", "", NULL};
  args[0] = "src/OS/outputs/1-print";
  if (fork() == 0) {
    printf("Child Process:\n");
    printf("PID: %d\n", getpid());
    printf("PPID: %d\n", getppid());
    printf("Executing another c program\n");
    execve(path, args, NULL);
  } else {
    printf("Parent Process : id = %d\n", getpid());
  }
}
