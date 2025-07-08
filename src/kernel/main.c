#include "assert.h"
#include "console.h"
#include "fork.h"
#include "ide.h"
#include "init.h"
#include "print.h"
#include "shell.h"
#include "stdio-kernel.h"
#include "stdio.h"
#include "syscall.h"

void init(void);

int main(void) {
  put_str("Welcome to Winx!\n");
  init_all();
  thread_exit(running_thread(), true);
  return 0;
}

void init(void) {
  uint32_t ret_pid = fork();
  if (ret_pid) {
    int status;
    int child_pid;
    while (1) {
      child_pid = wait(&status);
      printf("Recieved a thread\nPID: %d\nStatus: %d\n", child_pid, status);
    }
  } else {
    my_shell();
  }
  panic("init: should not be here");
}
