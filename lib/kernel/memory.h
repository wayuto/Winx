#ifndef __KERNEL_MEMORY_H
#define __KERNEL_MEMORY_H

#include "bitmap.h"
#include "list.h"
#include "stdint.h"

struct virtual_addr {
  struct bitmap vaddr_bitmap;
  uint32_t vaddr_start;
};

extern struct pool kernel_pool, user_pool;
void mem_init(void);

#define PG_P_1 1
#define PG_P_0 0
#define PG_RW_R 0
#define PG_RW_W 2
#define PG_US_S 0
#define PG_US_U 4

enum pool_flags { PF_KERNEL = 1, PF_USER = 2 };

struct mem_block {
  struct list_elem free_elem;
};

struct mem_block_desc {
  uint32_t block_size;
  uint32_t blocks_per_arena;
  struct list free_list;
};

#define DESC_CNT 7

void *get_kernel_pages(uint32_t pg_cnt);
void *malloc_page(enum pool_flags pf, uint32_t pg_cnt);
void malloc_init(void);
uint32_t *pte_ptr(uint32_t vaddr);
uint32_t *pde_ptr(uint32_t vaddr);
uint32_t addr_v2p(uint32_t vaddr);
void *get_user_pages(uint32_t pg_cnt);
void *get_a_page(enum pool_flags pf, uint32_t vaddr);
void block_desc_init(struct mem_block_desc *desc_array);
void *sys_malloc(uint32_t size);
void mfree_page(enum pool_flags pf, void *_vaddr, uint32_t pg_cnt);
void pfree(uint32_t pg_phy_addr);
void sys_free(void *ptr);
void *get_a_page_without_opvaddrbitmap(enum pool_flags pf, uint32_t vaddr);
void free_a_phy_page(uint32_t pg_phy_addr);
#endif
