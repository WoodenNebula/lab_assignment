#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (fork() == 0) {
    printf("Child Process!\n");
  } else {
    printf("Parent Process!\n");
  }
  printf("Hello Arch Linux!\n");
}
