


//TODO - kernel ZONE

ZONE和BUDDY系统


在内存分配策略中，同一个node的内存，位于hignmem zone中的会被优先分配，
当该zone的内存不满足条件时，就会从normal zone中分配，
只有当以上两个zone中内存都不满足条件时才会从dma zone分配

a.高水线（high）：如果内存区域的空闲页数大于高水线，说明内存区域的内存非常充足；
b.低水线（low）：如果内存区域的空闲页数小于低水线，说明内存区域的内存轻微不足；  触发kswapd
c.最低水线（min）：如果内存区域的空闲页数小于最低水线，说明内存区域的内存严重不足  触发 direct reclaim 阻塞等待kswapd

#define MAX_ORDER 11    /* 最多分配 4k * 2^11 */
#define MAX_NR_ZONES 4  /* 4个zone */

typedef struct pglist_data
        struct zone node_zones[MAX_NR_ZONES];
        struct task_struct *kswapd;

struct zone {
        struct free_area  free_area[MAX_ORDER];
}

struct free_area {
        struct list_head  free_list[MIGRATE_TYPES];   /* 从这个里面获取page 参考 __rmqueue_smallest */
        unsigned long    nr_free;
};

ZONE_DMA      /* 物理地址 */
ZONE_NORMAL   /* 物理地址 */
ZONE_HIGHMEM  /* 物理地址 */
ZONE_MOVABLE  /* 虚拟地址 物理内存来自上述三个ZONE */

free_area  /* 按照 order^2 的方式 组织struct page */
free_list  /* 基于 free_area 按照 MIGRATE_TYPES 将相同尺寸的内存再次组织 */


//TODO - kernel mm MIGRATE_TYPES
enum migratetype {
        MIGRATE_UNMOVABLE,     /* 不可移动页 内核线性映射 */
        MIGRATE_MOVABLE,       /* 可移动页 修改pte 迁移物理页 用户空间分配 */
        MIGRATE_RECLAIMABLE,   /* 不能被移动 但是 可以被回收swap 文件缓存 */
        MIGRATE_PCPTYPES,      /* per_cpu_pageset */
        MIGRATE_HIGHATOMIC = MIGRATE_PCPTYPES,         /* 紧急内存 */
#ifdef CONFIG_CMA
        MIGRATE_CMA,           /* 可以被buddy分配出去 但是CMA需要时就要回收 */
#endif
#ifdef CONFIG_MEMORY_ISOLATION
        MIGRATE_ISOLATE,        /* can't allocate from here */
#endif
        MIGRATE_TYPES          /* 不代表任何区域，只是单纯表示一共有多少个迁移类型 */
};


//TODO - kernel mm fallbacks rules

static int fallbacks[MIGRATE_TYPES][3] = {
	[MIGRATE_UNMOVABLE]   = { MIGRATE_RECLAIMABLE, MIGRATE_MOVABLE,   MIGRATE_TYPES },
	[MIGRATE_MOVABLE]     = { MIGRATE_RECLAIMABLE, MIGRATE_UNMOVABLE, MIGRATE_TYPES },
	[MIGRATE_RECLAIMABLE] = { MIGRATE_UNMOVABLE,   MIGRATE_MOVABLE,   MIGRATE_TYPES },
};