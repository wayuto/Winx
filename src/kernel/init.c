#include "init.h"

#include "console.h"
#include "fs.h"
#include "ide.h"
#include "interrupt.h"
#include "keyboard.h"
#include "memory.h"
#include "print.h"
#include "syscall-init.h"
#include "thread.h"
#include "timer.h"
#include "tss.h"

void init_all() {
  put_str("init_all\n");
  idt_init();
  mem_init();
  thread_init();
  timer_init();
  console_init();
  keyboard_init();
  tss_init();
  syscall_init();
  intr_enable();
  ide_init();
  filesys_init();

  extern void clear(void);
  clear();

  put_str(" _       ___               _____ __         ____\n"
          "| |     / (_)___  _  __   / ___// /_  ___  / / /\n"
          "| | /| / / / __ \\| |/_/   \\__ \\/ __ \\/ _ \\/ / /\n"
          "| |/ |/ / / / / />  <    ___/ / / / /  __/ / /\n"
          "|__/|__/_/_/ /_/_/|_|   /____/_/ /_/\___/_/_/\n");
}
