


//TODO - kernel mm doc

/* https://www.cnblogs.com/arnoldlu/p/8051674.html */

/* https://mp.weixin.qq.com/s?__biz=Mzg2MzU3Mjc3Ng==&mid=2247486732&idx=1&sn=435d5e834e9751036c96384f6965b328&chksm=ce77cb4bf900425d33d2adfa632a4684cf7a63beece166c1ffedc4fdacb807c9413e8c73f298&cur_album_id=2559805446807928833&scene=190#rd */



//TODO - kernel memory init from start

/* https://www.cnblogs.com/arnoldlu/p/8060121.html */



//TODO - linux memory 内存布局

0xffff8000  0xffffffff
用于 copy_user_page 和 clear_user_page

0xffff1000  0xffff7fff
保留 任何平台都不能使用该段 虚拟内存空间

0xffff0000  0xffff0fff
异常向量表 所在的内存区域

0xffc00000  0xffefffff
专用页面映射区(固定页面映射区) 使用 fix_to_vir()可以获取该区域的逻辑地址

0xfee00000  0xfeffffff
PCI技术的IO映射空间

VMALLOC_START  VMALLOC_END
使用 vmlloac() 和 ioremap() 获取的地址都处于该内存段

PAGE_OFFSET  high_memory-1
内存直接映射区域(常规内存映射区域) 该段内存地址也称为 逻辑地址  可以用于 DMA寻址

PKMAP_BASE  PAGE_OFFSET-1
持久映射区域 一般用于映射 高端内存

// TODO - [linux memory ks虚拟地址布局 32bits]

* 临时映射	[?, 0xFFFF_FFFF]

* 固定映射	[FIXADDR_START, FIXADDR_END]

* 永久映射	[PKMAP_BASE, FIXADDR_START]

* vmalloc	[VMALLOC_START, VMALLOC_END]

* normal	[3G+16M, 3G+892M]

* dma映射	[0xC00_0000+TASK_SIZE, 3G+16M]

* 用户态	[0, 3G]

// TODO - [linux memory us虚拟地址布局 32bits]

* .stack

* .mmap

* .heap

* .bss

* .data

* .text

* .reserved


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


//TODO - kernel mm buddy __GFP_*

/* 迁移类型 */
___GFP_RECLAIMABLE	分配的页面是可以回收
___GFP_MOVABLE		指定分配的页面是可以移动的
/* 管理区类型 */
__GFP_DMA		16bits
__GFP_DMA32		32bits
__GFP_NORMAL
__GFP_HIGHMEM
/* 重试类型 */
___GFP_RETRY_MAYFAIL	允许重试
__GFP_NORETRY		不允许重试
___GFP_NOFAIL 		直到成功
/* 紧急类型 */
___GFP_HIGH		内存分配请求是高优先级的  从紧急预留内存中分配
___GFP_MEMALLOC		允许内核在分配内存时可以从所有内存区域中获取内存 包括从紧急预留内存中获取 快用快放
___GFP_NOMEMALLOC 	禁止 从紧急预留内存中获取内存
___GFP_ATOMIC		不允许睡眠
/* 复合类型 */
___GFP_IO		分配时允许swap
___GFP_FS		允许内核执行底层文件系统操作
___GFP_ZERO		/* */
___GFP_DIRECT_RECLAIM	分配时 运行直接回收
___GFP_KSWAPD_RECLAIM	如果剩余内存容量在 _watermark[WMARK_MIN] 与 _watermark[WMARK_LOW] 之间时 内核就会唤醒 kswapd 进程开始异步内存回收
___GFP_NOWARN		分配失败时 抑制内核的错误报告
___GFP_HARDWALL 	当前numa分配
___GFP_THISNODE		NUMA 节点或者在指定 NUMA 节点中分配内存



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
	// 内核线程的 active_mm = init_mm; mm = NULL;
	// 普通进程的 active_mm = mm = 有意义的地址;
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


//TODO - [mm 物理内存 概念]

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



//TODO - [mm 页表属性 For 设备内存]

MT_DEVICE_nGnRnE
Non-Gathering (nG): 不支持写合并
Non-Reordering (nR): 禁止访问顺序重排
No Early Write Acknowledgement (nE): 同步写操作

MT_DEVICE_GRE
Gathering (G): 允许内存访问合并
Reordering (R): 允许访问重排序
Early Write Acknowledgement (E): 写操作可以在实际完成之前被认为已完成

// TODO - [mm 页表属性 For Normal内存]

Cacheable(可缓存)
Non-cacheable(不可缓存)
Write-Back(回写)
Write-Through(写通)


//TODO - swiotlb

低地址 <---> swiotlb映射表 <---> 高地址

1 设备硬件决定 只能访问低地址
2 cpu介入 将低地址数据拷贝到高地址空间
3 系统访问高地址数据


//TODO - kernel kworker

kworker/n:x             普通的per-cpu工作者线程        n表示CPU编号 x表示线程编号
kworker/n:xH            高优先级的per-cpu工作者线程     n表示CPU编号 x表示线程编号
kworker/u:x             非绑定的全局工作者线程          u表示非绑定 x表示线程编号。


// TODO - [mm Dram 读写的物理硬件行为]


page opened [buffer 命中]
	page hit
	cas

page closed [buffer 未命中]
	page miss
	ras	[列有效]
	cas	[行有效]

page conflict [当前访问的page和buffer不一致]
	pre-charge [关闭当前open]
	ras
	cas


