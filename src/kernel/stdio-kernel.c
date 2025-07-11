#include "stdio-kernel.h"

#include "console.h"
#include "global.h"
#include "stdio.h"

#define va_start(args, first_fix) args = (va_list) & first_fix
#define va_end(args) args = NULL

void printk(const char *format, ...) {
  va_list args;
  va_start(args, format);
  char buf[1024] = {0};
  vsprintf(buf, format, args);
  va_end(args);
  console_put_str(buf);
}
