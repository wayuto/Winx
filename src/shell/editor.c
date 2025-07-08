#include "fs.h"           
#include "shell.h"        
#include "stdio.h"        
#include "string.h"       
#include "user/syscall.h" 

#define MAX_LINES 100
#define MAX_LENGTH 256

char *lines[MAX_LINES];
int lineCount = 0;
char filename[256] = "";

extern void make_clear_abs_path(char *path, char *final_path);
extern char final_path[];


void read_line(char *buf, int size) {
  int i = 0;
  char c = 0;
  while (i < size - 1) {
    if (read(0, &c, 1) != 1 || c == '\n' || c == '\r') {
      if (c == '\n' || c == '\r')
        putchar('\n'); 
      break;
    }
    buf[i++] = c;
    putchar(c); 
  }
  buf[i] = 0;
}


int read_int() {
  char buf[32];
  read_line(buf, sizeof(buf));
  int val = 0;
  int neg = 0, i = 0;
  if (buf[0] == '-') {
    neg = 1;
    i++;
  }
  for (; buf[i]; i++) {
    if (buf[i] >= '0' && buf[i] <= '9')
      val = val * 10 + (buf[i] - '0');
    else
      break;
  }
  return neg ? -val : val;
}

void display() {
  printf("\nContent:\n");
  for (int i = 0; i < lineCount; i++) {
    printf("%d: %s", i + 1, lines[i]);
    int l = strlen(lines[i]);
    if (l == 0 || lines[i][l - 1] != '\n')
      putchar('\n');
  }
  printf("Total: %d lines\n", lineCount);
}

void openFile() {
  printf("File path: ");
  read_line(filename, sizeof(filename));
  make_clear_abs_path(filename, final_path); 
  int fd = open(final_path, O_RDONLY);
  if (fd < 0) {
    printf("Couldn't open file\n");
    return;
  }
  for (int i = 0; i < lineCount; i++)
    free(lines[i]);
  lineCount = 0;
  char buffer[MAX_LENGTH];
  int n, pos = 0;
  while ((n = read(fd, buffer + pos, 1)) == 1 && lineCount < MAX_LINES) {
    if (buffer[pos] == '\n' || pos == MAX_LENGTH - 2) {
      buffer[pos + (buffer[pos] == '\n' ? 0 : 1)] = 0;
      char *p = malloc(strlen(buffer) + 2);
      strcpy(p, buffer);
      int l = strlen(buffer);
      if (l == 0 || buffer[l - 1] != '\n') {
        p[l] = '\n';
        p[l + 1] = 0;
      }
      lines[lineCount++] = p;
      pos = 0;
    } else {
      pos++;
    }
  }
  if (pos > 0 && lineCount < MAX_LINES) {
    buffer[pos] = 0;
    char *p = malloc(strlen(buffer) + 2);
    strcpy(p, buffer);
    int l = strlen(buffer);
    if (l == 0 || buffer[l - 1] != '\n') {
      p[l] = '\n';
      p[l + 1] = 0;
    }
    lines[lineCount++] = p;
  }
  close(fd);
  printf("Loaded %d lines\n", lineCount);
}

void append() {
  if (!*filename) {
    printf("Open a file first\n");
    return;
  }
  printf("Contents (End with empty line):\n");
  char buffer[MAX_LENGTH];
  while (lineCount < MAX_LINES) {
    read_line(buffer, sizeof(buffer));
    if (buffer[0] == 0)
      break;
    char *p = malloc(strlen(buffer) + 2);
    strcpy(p, buffer);
    int l = strlen(buffer);
    if (l == 0 || buffer[l - 1] != '\n') {
      p[l] = '\n';
      p[l + 1] = 0;
    }
    lines[lineCount++] = p;
  }
}

void saveFile() {
  if (!*filename) {
    printf("Open a file first\n");
    return;
  }
  make_clear_abs_path(filename, final_path); 
  unlink(final_path);
  int fd = open(final_path, O_RDWR | O_CREAT);
  if (fd < 0) {
    printf("Couldn't save file\n");
    return;
  }
  for (int i = 0; i < lineCount; i++) {
    write(fd, lines[i], strlen(lines[i]));
  }
  close(fd);
  printf("Saved %d lines\n", lineCount);
}

int editor_main(int argc, char **argv) {
  int choice;
  while (1) {
    printf("1.Open 2.Show 3.Append 4.Save 5.Exit\n");
    printf("Option: ");
    choice = read_int();
    switch (choice) {
    case 1:
      openFile();
      break;
    case 2:
      display();
      break;
    case 3:
      append();
      break;
    case 4:
      saveFile();
      break;
    case 5:
      for (int i = 0; i < lineCount; i++)
        free(lines[i]);
      return 0;
    default:
      printf("Invalid option\n");
    }
  }
}
