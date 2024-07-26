


//TODO - kernel mm doc

/* https://www.cnblogs.com/arnoldlu/p/8051674.html */

/* https://mp.weixin.qq.com/s?__biz=Mzg2MzU3Mjc3Ng==&mid=2247486732&idx=1&sn=435d5e834e9751036c96384f6965b328&chksm=ce77cb4bf900425d33d2adfa632a4684cf7a63beece166c1ffedc4fdacb807c9413e8c73f298&cur_album_id=2559805446807928833&scene=190#rd */



//TODO - kernel memory init from start

/* https://www.cnblogs.com/arnoldlu/p/8060121.html */



//TODO - linux memory 内存布局


0xffff8000  0xffffffff
用于 copy_user_page 和 clear_user_page

0xffff1000  0xffff7fff
保留，任何平台都不能使用该段 虚拟内存空间

0xffff0000  0xffff0fff
异常向量表 所在的内存区域

0xffc00000  0xffefffff
专用页面映射区(固定页面映射区)，使用 fix_to_vir()可以获取该区域的逻辑地址

0xfee00000  0xfeffffff
PCI技术的IO映射空间

VMALLOC_START  VMALLOC_END
使用 vmlloac() 和 ioremap() 获取的地址都处于该内存段

PAGE_OFFSET  high_memory-1
内存直接映射区域(常规内存映射区域)，该段内存地址也称为 逻辑地址 ，可以用于 DMA寻址

PKMAP_BASE  PAGE_OFFSET-1
持久映射区域，一般用于映射 高端内存



//TODO - linux mmap

https://blog.csdn.net/hexiaolong2009/article/details/107592704

mmap的三种映射
系统调用前  申请PAGE 并且建立映射  remap_pfn_range
系统调用时  申请PAGE 不建立映射    vm_insert_page
pagefault时  不申请PAGE  alloc_page  vm_insert_page


/* 查看bar的io地址 */
cat /proc/ioports

/* 查看bar的内存空间 */
cat /proc/iomem


//TODO - kernel mm 64bit 虚拟内存布局

https://mp.weixin.qq.com/s?__biz=Mzg2MzU3Mjc3Ng==&mid=2247488477&idx=1&sn=f8531b3220ea3a9ca2a0fdc2fd9dabc6&chksm=ce77d59af9005c8c2ef35c7e45f45cbfc527bfc4b99bbd02dbaaa964d174a4009897dd329a4d&scene=21&cur_album_id=2559805446807928833#wechat_redirect


//TODO - kernel mm buddy GFP_*

/* 这两个标志会影响底层的伙伴系统从哪个区域中去获取空闲内存页 */
__GFP_RECLAIMABLE      分配的页面是可以回收
__GFP_MOVABLE          指定分配的页面是可以移动的
__GFP_HIGH             内存分配请求是高优先级的  从紧急预留内存中分配
__GFP_IO               内核在分配物理内存的时候可以发起磁盘 IO 操作 如果设置了该标志，表示允许内核将不常用的内存页置换出去
__GFP_FS               允许内核执行底层文件系统操作
__GFP_ZERO             将内存页初始化填充字节 0
__GFP_DIRECT_RECLAIM   在进行内存分配的时候，可以进行直接内存回收
__GFP_KSWAPD_RECLAIM   如果剩余内存容量在 _watermark[WMARK_MIN] 与 _watermark[WMARK_LOW] 之间时，内核就会唤醒 kswapd 进程开始异步内存回收
__GFP_NOWARN           抑制内核的分配失败错误报告
__GFP_RETRY_MAYFAIL    分配内存失败的时候，允许重试
__GFP_NORETRY          分配内存失败时不允许重试
__GFP_NOFAIL           一直重试直到成功为止
__GFP_HARDWALL         只能在当前进程分配到的 CPU 所关联的 NUMA 节点上进行分配 当进程可以运行的 CPU 受限时，该标志才会有意义 绑核
__GFP_THISNODE         内核分配内存的行为只能在当前 NUMA 节点或者在指定 NUMA 节点中分配内存
__GFP_MEMALLOC         允许内核在分配内存时可以从所有内存区域中获取内存，包括从紧急预留内存中获取 快用快放
__GFP_NOMEMALLOC       明确禁止内核从紧急预留内存中获取内存


enum {
    ___GFP_DMA_BIT,
    ___GFP_HIGHMEM_BIT,
    ___GFP_DMA32_BIT,
    ___GFP_MOVABLE_BIT,
    ___GFP_RECLAIMABLE_BIT,
    ___GFP_HIGH_BIT,
    ___GFP_IO_BIT,
    ___GFP_FS_BIT,
    ___GFP_ZERO_BIT,
    ___GFP_UNUSED_BIT,	/* 0x200u unused */
    ___GFP_DIRECT_RECLAIM_BIT,
    ___GFP_KSWAPD_RECLAIM_BIT,
    ___GFP_WRITE_BIT,
    ___GFP_NOWARN_BIT,
    ___GFP_RETRY_MAYFAIL_BIT,
    ___GFP_NOFAIL_BIT,
    ___GFP_NORETRY_BIT,
    ___GFP_MEMALLOC_BIT,
    ___GFP_COMP_BIT,
    ___GFP_NOMEMALLOC_BIT,
    ___GFP_HARDWALL_BIT,
    ___GFP_THISNODE_BIT,
    ___GFP_ACCOUNT_BIT,
    ___GFP_ZEROTAGS_BIT,
#ifdef CONFIG_KASAN_HW_TAGS
    ___GFP_SKIP_ZERO_BIT,
    ___GFP_SKIP_KASAN_BIT,
#endif
#ifdef CONFIG_LOCKDEP
    ___GFP_NOLOCKDEP_BIT,
#endif
    ___GFP_LAST_BIT
};


#define GFP_HIGHUSER	(GFP_USER | __GFP_HIGHMEM)
#define GFP_USER	(__GFP_RECLAIM | __GFP_IO | __GFP_FS | __GFP_HARDWALL)



//TODO - kernel mm walk page

walk_page_range()  /* ks */
lookup_address()   /* ks */
walk_page_range_novma() /* us */


//TODO - kernel mm zoneinfo

free = nr_free_pages /* 空闲的page */
min /* 水位线 */
low
high
nr_zone_active_anon   /* 活跃匿名page */
nr_zone_inactive_anon /* 非活跃匿名page */
nr_zone_active_file   /* 活跃文件page */
nr_zone_inactive_file /* 非活跃文件page */
spanned   /* 加空洞的所有page */
presented /* 不加空洞的所有page */
managed   /* */
protection /* 保留内存 方式向下压缩 */


min_free_kbytes == WMARK_MIN
WMARK_MIN * 1.25 = WMARK_LOW
WMARK_LOW * 1.5  = WMARK_HIGH


//TODO - kernel mm smaps

rwxp                            /* 读 写 执行 私有 */
Size                            /* 虚拟内存大小 */
KernelPageSize                  /* 内核页大小 */
MMUPageSize                     /* MMU页大小 */
VSS Virtual Set Size            /* 虚拟耗用内存(包含共享库) */
RSS (RES) Resident Set Size           /* 实际使用物理内存(共享库) */
PSS Proportional Set Size       /* 实际使用的物理内存(比例分配 共享库) */
USS - Unique Set Size           /* 进程独自占用的物理内存(不包含共享库) */
VSS >= RSS >= PSS >= USS
Pss_Dirty
Anonymous
LazyFree
Referenced
Locked /* 防止swap out */


//TODO - kernel mm swappiness

文件页(代价低) -> swappiness -> 匿名页(代价高)


//TODO - kernel task_struct

 struct task_struct {
        // 内核线程的 active_mm 指向前一个进程的地址空间
        // 普通进程的 active_mm 指向 null
        struct mm_struct *active_mm;
}

struct mm_struct {
        struct vm_area_struct *mmap;        /* list of VMAs */
        struct rb_root mm_rb;               /* all vma */
        pgd_t * pgd;
        int map_count;                      /* count of vma */
}

struct vm_area_struct {
        unsigned long vm_start;         /* addr */
        unsigned long vm_end;           /* addr + len */
        struct mm_struct *vm_mm;
        pgprot_t vm_page_prot;
        struct vm_area_struct *vm_next, *vm_prev;   /* vma 在 mm_struct->mmap 双向链表中的前驱节点和后继节点 */
        struct rb_node vm_rb;                       /* vma 在 mm_struct->mm_rb 红黑树中的节点 */
}


//TODO - mm 物理内存 概念

wafer           /* 晶圆 */
die             /* 晶圆上 切割的一个 小方块 */
channel
dimm            /* 双列直插 */
rank
bank
row
column
va -> pa 解析 包含:
        channel id
        rank id         /* 正方体 */
        bank id         /* 正方体的一个切面 */
        row  id
        column id



//TODO - mm 页表属性

MT_DEVICE_nGnRnE
Non-Gathering (nG): 禁止内存访问合并。每个内存访问都独立处理，不会合并多个访问请求。
Non-Reordering (nR): 禁止访问顺序重排序。保持内存访问顺序，不会因为优化而改变顺序。
No Early Write Acknowledgement (nE): 禁止早期写确认。写操作必须实际完成后才能被确认。

MT_DEVICE_GRE
Gathering (G): 允许内存访问合并。多次内存访问可以被合并，以提高性能。
Reordering (R): 允许访问重排序。内存访问顺序可以被重新安排，以优化性能。
Early Write Acknowledgement (E): 允许早期写确认。写操作可以在实际完成之前被认为已完成。