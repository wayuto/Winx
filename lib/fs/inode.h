#ifndef __FS_INODE_H
#define __FS_INODE_H
#include "ide.h"
#include "list.h"
#include "stdint.h"

struct inode {
  uint32_t i_no;
  uint32_t i_size;
  uint32_t user_id;
  uint64_t ctime;
  uint64_t mtime;
  uint64_t atime;
  uint32_t privilege;
  uint32_t i_open_cnts;
  bool write_deny;
  uint32_t i_sectors[13];
  struct list_elem inode_tag;
};

struct inode *inode_open(struct partition *part, uint32_t inode_no);
void inode_close(struct inode *inode);
void inode_sync(struct partition *part, struct inode *inode);
void inode_init(uint32_t inode_no, struct inode *new_inode);
void inode_delete(struct partition *part, uint32_t inode_no, void *io_buf);
void inode_release(struct partition *part, uint32_t inode_no);

#endif
