
//TODO - kasan doc

http://www.wowotech.net/memory_management/424.html


//TODO - kasan 原理

使用 1/8 内存 shadow memory标记剩余可用内存
连续8 Byte可以访问         标记0
0 < X < 8 Byte可以访问     标记 X
8 bytes内存访问都是invalid  shadow memory的值为负数


out-of-bounds     oob
use-after-free    uaf


/* padding 区域 填充字段 意义 */
#define KASAN_FREE_PAGE         0xFF  /* page was freed */
#define KASAN_PAGE_REDZONE      0xFE  /* redzone for kmalloc_large allocations */
#define KASAN_KMALLOC_REDZONE   0xFC  /* redzone inside slub object */
#define KASAN_KMALLOC_FREE      0xFB  /* object was freed (kmem_cache_free/kfree) */
#define KASAN_GLOBAL_REDZONE    0xFA  /* redzone for global variable */


//TODO - kasan kernel config

CONFIG_SLUB_DEBUG=y
CONFIG_KASAN=y


/* 编译器插入 __asan_load##size() 和 __asan_store##size() */

mov x0, #0x5678
movk x0, #0x1234, lsl #16
movk x0, #0x8000, lsl #32
movk x0, #0xffff, lsl #48
mov w1, #0x5       /* 0xffff800012345678 写入 5 */
bl __asan_store1   /* 访问 1 Byte __asan_storeX 访问 X byte 编译器插入 查询shadow memory */
strb w1, [x0]      /* 实际访问发生 */



//TODO - kasan C 代码模拟 psudo code

long *addr = (long *)0xffff800012345678;
char *shadow = (char *)(((unsigned long)addr >> 3) + KASAN_SHADOW_OFFSE);
if (*shadow)   /* 如果是1-7Byte  if (*shadow && *shadow < ((unsigned long)addr & 7) + N); //N = 1,2,4 */
	report_bug();
*addr = 0;


//TODO - kasan kernel 地址空间

VA_BITS = 48
ks 空间 256TB
shadow memory 空间 32TB

ks 起始地址 0xffff_0000_0000_0000

ks 空间虚拟地址分配
	KASAN
	MODULE
	FIXMAP
	VMALLOC
	PCI_IO
	VMEMMAP
	linear mapping



//TODO - kasan 缺点

1 需要编译器支持 插入检查点
2 kasan 需要占用1/9 内存 用1Byte标记8Byte
3 kasan 将内存分成 合法 / 非法 如果访问了其他合法地址 踩内存 并不能被检测


//TODO - movsb

movsb
源地址 DS:SI
目的地址 ES:DI
DF=0 正向; DF=1 反向;
SI和DI 加/减1;


//TODO - lgdt lidt

加载 gdt idt 地址

