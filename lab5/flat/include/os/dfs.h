#ifndef __DFS_H__
#define __DFS_H__

#include "dfs_shared.h"

typedef unsigned int uint32;

#define DFS_GET_BLOCK_NUM(block_entry) ((block_entry) >> 1)
#define DFS_IS_BLOCK_ENTRY_VALID(block_entry) ((block_entry) & 0x1)
#define DFS_SET_BLOCK_ENTRY(blocknum, valid) (((blocknum) << 1) | (valid))

#endif
