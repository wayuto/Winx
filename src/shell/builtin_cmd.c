#include "builtin_cmd.h"

#include <stdint.h>

#include "assert.h"
#include "dir.h"
#include "fs.h"
#include "shell.h"
#include "stdio.h"
#include "string.h"
#include "syscall.h"

static void wash_path(char *old_abs_path, char *new_abs_path) {
    assert(old_abs_path[0] == '/');
    char name[MAX_FILE_NAME_LEN] = {0};
    char *sub_path = old_abs_path;
    sub_path = path_parse(sub_path, name);
    if (name[0] == 0) {
        new_abs_path[0] = '/';
        new_abs_path[1] = 0;
        return;
    }
    new_abs_path[0] = 0;
    strcat(new_abs_path, "/");
    while (name[0]) {
        if (!strcmp("..", name)) {
            char *slash_ptr = strrchr(new_abs_path, '/');

            if (slash_ptr != new_abs_path) {
                *slash_ptr = 0;
            } else {
                *(slash_ptr + 1) = 0;
            }
        } else if (strcmp(".", name)) {
            if (strcmp(new_abs_path, "/")) {
                strcat(new_abs_path, "/");
            }
            strcat(new_abs_path, name);
        }

        memset(name, 0, MAX_FILE_NAME_LEN);
        if (sub_path) {
            sub_path = path_parse(sub_path, name);
        }
    }
}

void make_clear_abs_path(char *path, char *final_path) {
    char abs_path[MAX_PATH_LEN] = {0};

    if (path[0] != '/') {
        memset(abs_path, 0, MAX_PATH_LEN);
        if (getcwd(abs_path, MAX_PATH_LEN) != NULL) {
            if (!((abs_path[0] == '/') && (abs_path[1] == 0))) {
                strcat(abs_path, "/");
            }
        }
    }
    strcat(abs_path, path);
    wash_path(abs_path, final_path);
}

void builtin_pwd(uint32_t argc, char **argv UNUSED) {
    if (argc != 1) {
        printf("pwd: no argument support!\n");
        return;
    } else {
        if (NULL != getcwd(final_path, MAX_PATH_LEN)) {
            printf("%s\n", final_path);
        } else {
            printf("pwd: get current work directory failed.\n");
        }
    }
}

char *builtin_cd(uint32_t argc, char **argv) {
    if (argc > 2) {
        printf("cd: only support 1 argument!\n");
        return NULL;
    }

    if (argc == 1) {
        final_path[0] = '/';
        final_path[1] = 0;
    } else {
        make_clear_abs_path(argv[1], final_path);
    }

    if (chdir(final_path) == -1) {
        printf("cd: no such directory %s\n", final_path);
        return NULL;
    }
    return final_path;
}

void builtin_ls(uint32_t argc, char **argv) {
    char *pathname = NULL;
    struct stat file_stat;
    memset(&file_stat, 0, sizeof(struct stat));
    bool long_info = false;
    uint32_t arg_path_nr = 0;
    uint32_t arg_idx = 1;
    while (arg_idx < argc) {
        if (argv[arg_idx][0] == '-') {
            if (!strcmp("-l", argv[arg_idx])) {
                long_info = true;
            } else if (!strcmp("-h", argv[arg_idx])) {
                printf("usage: -l list all infomation about the file.\n-h for "
                       "help\nlist all files in the current dirctory if no "
                       "option\n");
                return;
            } else {
                printf("ls: invalid option %s\nTry `ls -h' for more "
                       "information.\n",
                       argv[arg_idx]);
                return;
            }
        } else {
            if (arg_path_nr == 0) {
                pathname = argv[arg_idx];
                arg_path_nr = 1;
            } else {
                printf("ls: only support one path\n");
                return;
            }
        }
        arg_idx++;
    }

    if (pathname == NULL) {
        if (NULL != getcwd(final_path, MAX_PATH_LEN)) {
            pathname = final_path;
        } else {
            printf("ls: getcwd for default path failed\n");
            return;
        }
    } else {
        make_clear_abs_path(pathname, final_path);
        pathname = final_path;
    }

    if (stat(pathname, &file_stat) == -1) {
        printf("ls: cannot access %s: No such file or directory\n", pathname);
        return;
    }
    if (file_stat.st_filetype == FT_DIRECTORY) {
        struct dir *dir = opendir(pathname);
        struct dir_entry *dir_e = NULL;
        char sub_pathname[MAX_PATH_LEN] = {0};
        uint32_t pathname_len = strlen(pathname);
        uint32_t last_char_idx = pathname_len - 1;
        memcpy(sub_pathname, pathname, pathname_len);
        if (sub_pathname[last_char_idx] != '/') {
            sub_pathname[pathname_len] = '/';
            pathname_len++;
        }
        rewinddir(dir);
        if (long_info) {
            char ftype;
            printf("total: %d\n", file_stat.st_size);
            while ((dir_e = readdir(dir))) {
                ftype = 'd';
                if (dir_e->f_type == FT_REGULAR) {
                    ftype = '-';
                }
                sub_pathname[pathname_len] = 0;
                strcat(sub_pathname, dir_e->filename);
                memset(&file_stat, 0, sizeof(struct stat));
                if (stat(sub_pathname, &file_stat) == -1) {
                    printf("ls: cannot access %s: No such file or directory\n",
                           dir_e->filename);
                    return;
                }
                printf("%c  %d  %d  %s\n", ftype, dir_e->i_no,
                       file_stat.st_size, dir_e->filename);
            }
        } else {
            while ((dir_e = readdir(dir))) {
                printf("%s ", dir_e->filename);
            }
            printf("\n");
        }
        closedir(dir);
    } else {
        if (long_info) {
            printf("-  %d  %d  %s\n", file_stat.st_ino, file_stat.st_size,
                   pathname);
        } else {
            printf("%s\n", pathname);
        }
    }
}

void builtin_ps(uint32_t argc, char **argv UNUSED) {
    if (argc != 1) {
        printf("ps: no argument support!\n");
        return;
    }
    ps();
}

void builtin_clear(uint32_t argc, char **argv UNUSED) {
    if (argc != 1) {
        printf("clear: no argument support!\n");
        return;
    }
    clear();
}

int32_t builtin_mkdir(uint32_t argc, char **argv) {
    int32_t ret = -1;
    if (argc != 2) {
        printf("mkdir: only support 1 argument!\n");
    } else {
        make_clear_abs_path(argv[1], final_path);

        if (strcmp("/", final_path)) {
            if (mkdir(final_path) == 0) {
                ret = 0;
            } else {
                printf("mkdir: create directory %s failed.\n", argv[1]);
            }
        }
    }
    return ret;
}

int32_t builtin_rmdir(uint32_t argc, char **argv) {
    int32_t ret = -1;
    if (argc != 2) {
        printf("rmdir: only support 1 argument!\n");
    } else {
        make_clear_abs_path(argv[1], final_path);

        if (strcmp("/", final_path)) {
            if (rmdir(final_path) == 0) {
                ret = 0;
            } else {
                printf("rmdir: remove %s failed.\n", argv[1]);
            }
        }
    }
    return ret;
}

int32_t builtin_rm(uint32_t argc, char **argv) {
    int32_t ret = -1;
    if (argc != 2) {
        printf("rm: only support 1 argument!\n");
    } else {
        make_clear_abs_path(argv[1], final_path);

        if (strcmp("/", final_path)) {
            if (unlink(final_path) == 0) {
                ret = 0;
            } else {
                printf("rm: delete %s failed.\n", argv[1]);
            }
        }
    }
    return ret;
}

void builtin_echo(uint32_t argc, char **argv) {
    if (argc == 1) putchar('\n');
    if (argc == 2) {
        printf("%s\n", argv[1]);
        return;
    }
    int i = 1;
    while (i < argc) {
        printf("%s", argv[i]);
        if (i == argc - 1) {
            putchar('\n');
            break;
        }
        printf(" ");
        i++;
    }
}

int32_t builtin_touch(uint32_t argc, char **argv) {
    int32_t ret = -1;
    if (argc != 2) {
        printf("touch: only support 1 argument!\n");
    } else {
        make_clear_abs_path(argv[1], final_path);
        if (strcmp("/", final_path)) {
            int32_t fd = open(final_path, O_CREAT | O_RDWR);
            if (fd != -1) {
                close(fd);
                ret = 0;
            } else {
                printf("touch: create file %s failed.\n", argv[1]);
            }
        }
    }
    return ret;
}

int32_t builtin_cat(uint32_t argc, char **argv) {
    if (argc != 2) {
        printf("cat: only support 1 argument!\n");
        return -1;
    }
    make_clear_abs_path(argv[1], final_path);
    int32_t fd = open(final_path, O_RDONLY);
    if (fd == -1) {
        printf("cat: cannot open %s\n", argv[1]);
        return -1;
    }
    char buf[128];
    int32_t bytes_read;
    while ((bytes_read = read(fd, buf, sizeof(buf))) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            putchar(buf[i]);
        }
    }
    putchar('\n');
    close(fd);
    return 0;
}

void builtin_help(uint32_t argc UNUSED, char **argv UNUSED) {
    printf("\
 builtin commands:\n\
       ls: show directory or file information\n\
       cd: change current work directory\n\
       mkdir: create a directory\n\
       rmdir: remove a empty directory\n\
       rm: remove a regular file\n\
       touch: create an empty file\n\
       echo: print text to the screen\n\
       pwd: show current work directory\n\
       ps: show process information\n\
       clear: clear screen\n\
       cat: read and print file content\n\
       putin: append text to file\n\
       help: show this help message\n\
   shortcut key:\n\
       ctrl+l: clear screen\n\
       ctrl+u: clear input\n");
}

int32_t builtin_putin(uint32_t argc, char **argv) {
    if (argc != 3) {
        printf("putin: usage: putin <file> <text>\n");
        return -1;
    }
    make_clear_abs_path(argv[1], final_path);
    int32_t fd = open(final_path, O_RDWR);
    if (fd == -1) {
        fd = open(final_path, O_CREAT | O_RDWR);
        if (fd == -1) {
            printf("putin: cannot open or create %s\n", argv[1]);
            return -1;
        }
    }

    struct stat st;
    if (stat(final_path, &st) == 0 && st.st_size > 0) {
        write(fd, "\n", 1);
    }
    write(fd, argv[2], strlen(argv[2]));
    close(fd);
    return 0;
}