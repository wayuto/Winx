#ifndef __THREAD_THREAD_H
#define __THREAD_THREAD_H
#include "list.h"
#include "memory.h"
#include "stdint.h"

#define MAX_FILES_OPEN_PER_PROC 8
#define TASK_NAME_LEN 16

typedef uint16_t pid_t;

typedef void thread_func(void *);

enum task_status {
    TASK_RUNNING,
    TASK_READY,
    TASK_BLOCKED,
    TASK_WAITING,
    TASK_HANGING,
    TASK_DIED
};

struct intr_stack {
    uint32_t vec_no;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_dummy;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;

    uint32_t err_code;
    void (*eip)(void);
    uint32_t cs;
    uint32_t eflags;
    void *esp;
    uint32_t ss;
};

struct thread_stack {
    uint32_t ebp;
    uint32_t ebx;
    uint32_t edi;
    uint32_t esi;

    void (*eip)(thread_func *func, void *func_arg);

    void(*unused_retaddr);
    thread_func *function;
    void *func_arg;
};

struct task_struct {
    uint32_t *self_kstack;
    pid_t pid;
    enum task_status status;
    uint8_t priority;
    char name[16];

    uint8_t ticks;
    uint32_t elapsed_ticks;
    struct list_elem general_tag;
    struct list_elem all_list_tag;
    uint32_t *pgdir;
    struct virtual_addr userprog_vaddr;
    int32_t fd_table[MAX_FILES_OPEN_PER_PROC];
    uint32_t cwd_inode_nr;
    int16_t parent_pid;
    struct mem_block_desc u_block_desc[DESC_CNT];
    int8_t exit_status;
    uint32_t stack_magic;
};

extern struct list thread_ready_list;
extern struct list thread_all_list;
void thread_create(struct task_struct *pthread, thread_func function,
                   void *func_arg);
void init_thread(struct task_struct *pthread, char *name, int prio);
struct task_struct *thread_start(char *name, int prio, thread_func function,
                                 void *func_arg);

struct task_struct *running_thread(void);
void schedule(void);
void thread_init(void);
void thread_block(enum task_status stat);
void thread_unblock(struct task_struct *pthread);
void thread_yield(void);
pid_t fork_pid(void);
void sys_ps(void);
void thread_exit(struct task_struct *thread_over, bool need_schedule);
struct task_struct *pid2thread(int32_t pid);
void release_pid(pid_t pid);
#endif
