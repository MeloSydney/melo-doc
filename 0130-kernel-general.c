

//TODO - kernel dynamic log pr_debug

pr_debug
dev_debug

mount -t debugfs none /sys/kernel/debug
cat /sys/kernel/debug/dynamic_debug/control

echo 'file svcsock.p +p' > /sys/kernel/debug/dynamic_debug/control
echo 'module hello +p' > /sys/kernel/debug/dynamic_debug/control
echo 'func svc_process() +p' > /sys/kernel/debug/dynamic_debug/control
echo -n '*usb* +p' > /sys/kernel/debug/dynamic_debug/control
echo -n '+p' > /sys/kernel/debug/dynamic_debug/control



// TODO - [kernel Dts delete-property]

删除node/property

/ {
	...
	ocp {
		...
		rtc: **rtc@48838000** {
			compatible = "ti,am3352-rtc";
			reg = <0x48838000 0x100>;
			interrupts = <GIC_SPI 217 IRQ_TYPE_LEVEL_HIGH>,
			<GIC_SPI 217 IRQ_TYPE_LEVEL_HIGH>;
			ti,hwmods = "rtcss";
			clocks = <&sys_32k_ck>;
			**property1 **= <1>;
			property2;
		};
	};
};
/ {
	...
	ocp {
		/delete-node/ rtc@48838000;  // 删除node
	};
};

&rtc {
	/delete-property/ **property1**;  // 删除属性
	/delete-property/ property2;
};

// TODO - [kernel Dts Interrupts]

ranges:
	ranges = <0 0x802000 0x1000>;			/* 0 base len */
	ranges = <local地址, parent地址, size>;		/* 表示将local地址向parent地址的转换 */
	reg = <0x00001 0x01 0x000010 0x02>;		/* 起始地址为0x0001 大小为0x01 起始地址为0x000010 大小为0x02 */

interrupts = <0 2 4>;
0 中断来源
2 中断线号
4 触发类型


//TODO - kernel dts 获取 ranges
static int demo_range_probe(struct platform_device *pdev)
{
	struct resource *res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	printk(KERN_INFO "%s start: 0x%x, end: 0x%x\n",
		res->name, res->start, res->end + 1);
	return 0;
}

[root@vexpress mnt]# insmod demo_range.ko
[  382.940402] range0 start: 0x3000100, end: 0x3000300
[  382.940697] range1 start: 0x3000300, end: 0x3000500
[  382.941448] range2 start: 0x3000600, end: 0x3000800
[  382.941657] range3 start: 0x3001100, end: 0x3001300
[  382.941855] range4 start: 0x3001400, end: 0x3001600
[  382.942057] range5 start: 0x3001600, end: 0x3001700
[  382.942262] range6 start: 0x3001750, end: 0x3001780
[  382.942470] range7 start: 0x3001730, end: 0x3001740
[  382.942684] range8 start: 0x3001720, end: 0x3001730
[  382.949796] range9 start: 0x3001800, end: 0x3001850
[  382.950023] range10 start: 0x3001900, end: 0x3001950
[  382.950603] range11 start: 0x3001950, end: 0x3001980
[  382.950805] range12 start: 0x3002000, end: 0x3003000


// TODO - [kernel Dts Lable]

dts 别名 label
[label:] node-name[@unit-address] {
	[properties definitions]
	[child nodes]
};



//TODO - [kernel mm API 地址转换]

//把内核态虚拟地址转成物理地址
#define __virt_to_phys(x)   (((phys_addr_t)(x) - PAGE_OFFSET + PHYS_OFFSET))

//把物理内存地址转成内核态虚拟地址
#define __phys_to_virt(x)   ((unsigned long)((x) - PHYS_OFFSET + PAGE_OFFSET))

//把页帧转成内存单元对应的page
#define __pfn_to_page(pfn)  (mem_map + ((pfn) - ARCH_PFN_OFFSET))

//把内存单元对应的page转成页帧
#define __page_to_pfn(page) ((unsigned long)((page) - mem_map) + ARCH_PFN_OFFSET)

//把内核虚拟地址转成页帧
#define virt_to_pfn(kaddr)  (__pa(kaddr) >> PAGE_SHIFT)

//把页帧转成内核虚拟地址
#define pfn_to_virt(pfn)    __va((pfn) << PAGE_SHIFT)

//把内核虚拟地址转成其内存单元对应page
#define virt_to_page(addr)  pfn_to_page(virt_to_pfn(addr))

//把内存单元对应page转成内核虚拟地址
#define page_to_virt(page)  pfn_to_virt(page_to_pfn(page))

//线性映射
#define __pa(x)         __virt_to_phys((unsigned long)(x))

//线性映射
#define __va(x)         ((void *)__phys_to_virt((phys_addr_t)(x)))

/* 虚拟地址转物理页号 */
virt_to_pfn()

/* 物理页号转虚拟地址 */
pfn_to_virt()

/* 物理地址转物理页号 */
phys_to_pfn()
__phys_to_pfn()
PHYS_PFN()

/* 物理页号转物理地址 */
pfn_to_phys()
__pfn_to_phys()
PFN_PHYS()

// TODO - [kernel API container_of]

container_of(obj, struct drm_gem_shmem_object, base)
a# 成员指针地址
b# 包含a#的结构体
c# a#在b#中的成员名


// TODO - [kernel commandline]

BOOT_IMAGE=/boot/vmlinuz-5.13.0-51-generic
	root=UUID=a407f6af-3baf-432f-b973-17e600ca8128
	ro
	quiet  此选项告诉内核不产生任何输出(a.k.a 非详细模式)
		如果在没有此选项的情况下启动，您将看到许多内核消息，
		例如驱动程序/模块激活，文件系统检查和错误 当您需要查找错误时，没有quiet参数可能很有用
	splash  此选项用于启动eye-candy “loading”屏幕，同时系统的所有核心部分都在后台加载
		如果您禁用它并启用quiet，您将获得一个空白屏幕
	loglevel=8             /* 日志等级 全部打印 */
	video=DP-1:800x600@60  /* 分辨率 刷新率 */
	vt.handoff=7
	nomodeset              /* 告诉内核在系统启动并运行之前不启动视频驱动程序 */

// TODO - [kernel mem_map]

alloc_node_mem_map(struct pglist_data *pgdat)
	start = pgdat->node_start_pfn & ~(MAX_ORDER_NR_PAGES - 1);
	end = pgdat->node_start_pfn + pgdat->node_spanned_pages;
	end = ALIGN(end, MAX_ORDER_NR_PAGES);
	size =  (end - start) * sizeof(struct page);
	map = alloc_remap(pgdat->node_id, size);
	pgdat->node_mem_map = map + (pgdat->node_start_pfn - start);

1 uma架构下 全部struct page被全局mem_map[]管理

2 numa架构下 当前node的struct page被当前node的node_mem_map[]管理

// TODO - [linux KO disagrees about version of symbols]

disagrees about version of symbols get_chnl_exporter  /* 和最后提交hash相关 */
disagrees about version of module_layout  ?

1 整编
2 从 work-shared 拷贝源码
3 从 work 拷贝 .config
4 如果有新的 git commit 需要重新执行123

// TODO - [kernel Lds 链接脚本]

https://blog.csdn.net/sinat_22338935/article/details/138086943

// TODO - kernel # ## 井号 字符串 拼接

#define DRM_IOCTL_DEF_DRV(ioctl, _func, _flags)        \
	[DRM_IOCTL_NR(DRM_IOCTL_##ioctl) - DRM_COMMAND_BASE] = {  \
		.cmd = DRM_IOCTL_##ioctl, \
		.func = _func,            \
		.flags = _flags,          \
		.name = #ioctl            \
	}

1 #    字符串化
2 ##   拼接字符串

#define drmm_add_action(dev, action, data) \
	__drmm_add_action(dev, action, data, #action)


// TODO - [kernel 编译 targz]

https://projectacrn.github.io/latest/tutorials/tar_installation.html
make clean; make -j 128 INSTALL_MOD_STRIP=1 targz-pkg



//TODO - [kernel panic pstate]

pstate: 20400009 (nzCv daif +PAN -UAO -TCO BTYPE=--)



//TODO - [kernel mm oom]

echo -1000 > /proc/2000/oom_score_adj /* 禁止oom杀掉 */



//TODO - [kernel Makefile 编译 module ko]

obj-m:= memdev.o // 编译目标
KERNELDIR:=/lib/modules/3.13.11-ckt39/build // 内核源码树
PWD:=$(shell pwd) // 当前工作目录
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
clean:
	rm -rf *.o *.mod.c *.mod.o *.ko


/* Makefile debug */
$(info “here add the debug info”)
$(warning “here add the debug info”)
$(error “error: this will stop the compile”)


//TODO -[ kernel Modprobe Format Error]

将源码中的Makefile文件修改为当前运行内核的版本号即可：

VERSION = 5
PATCHLEVEL = 3
SUBLEVEL = 0
EXTRAVERSION = -45-generic

https://www.cnblogs.com/haiyonghao/p/14440115.html


//TODO - [kernel 解压 vmlinuz]

dd if=/boot/vmlinuz-$(uname -r) bs=1 skip=13932 | zcat > vmlinux-$(uname -r)


//TODO - kernel memory port io

不占用CPU的物理地址空间
通过IN/OUT指令访问



//TODO - [kernel Gpio general]

of_get_named_gpio_flags()
led-green = <&gpio1 8 GPIO_ACTIVE_LOW>;  /* #1 bank  #2 pinnum  #3 flags */



//TODO - kernel initcall general

#define INITCALLS                                           \
     *(.initcallearly.init)                                        \
     VMLINUX_SYMBOL(__early_initcall_end) = .;                  \
     *(.initcall0.init)                                       \
     *(.initcall0s.init)                                     \
     *(.initcall1.init)                                       \
     *(.initcall1s.init)                                     \
     *(.initcall2.init)                                       \
     *(.initcall2s.init)                                     \
     *(.initcall3.init)                                       \
     *(.initcall3s.init)                                     \
     *(.initcall4.init)                                       \
     *(.initcall4s.init)                                     \
     *(.initcall5.init)                                       \
     *(.initcall5s.init)                                     \
     *(.initcallrootfs.init)                                      \
     *(.initcall6.init)                                       \
     *(.initcall6s.init)                                     \
     *(.initcall7.init)                                       \
     *(.initcall7s.init)

#define pure_initcall(fn)          __define_initcall("0",fn,0)
#define core_initcall(fn)          __define_initcall("1",fn,1)
#define core_initcall_sync(fn)     __define_initcall("1s",fn,1s)
#define postcore_initcall(fn)      __define_initcall("2",fn,2)
#define postcore_initcall_sync(fn) __define_initcall("2s",fn,2s)
#define arch_initcall(fn)          __define_initcall("3",fn,3)
#define arch_initcall_sync(fn)     __define_initcall("3s",fn,3s)
#define subsys_initcall(fn)        __define_initcall("4",fn,4)
#define subsys_initcall_sync(fn)   __define_initcall("4s",fn,4s)
#define fs_initcall(fn)            __define_initcall("5",fn,5)
#define fs_initcall_sync(fn)       __define_initcall("5s",fn,5s)
#define rootfs_initcall(fn)        __define_initcall("rootfs",fn,rootfs)
#define device_initcall(fn)        __define_initcall("6",fn,6)
#define device_initcall_sync(fn)   __define_initcall("6s",fn,6s)
#define late_initcall(fn)          __define_initcall("7",fn,7)
#define late_initcall_sync(fn)     __define_initcall("7s",fn,7s)


//TODO - [kernel softirq 软中断]

/* https://www.cnblogs.com/arnoldlu/p/8659986.html */
/* 软中断 执行时机 */
irq_exit()->invoke_softirq()->wakeup_softirq()->唤醒 ksoftirqd
local_bh_enable()->__local_bh_enable()->do_softirq()
	->__do_softirq(CONFIG_PREEMPT_RT_FULL)->wakeup_softirq()->唤醒 ksoftirqd
ksoftirqd: run_ksoftirqd()
		__do_softirq()



//TODO - kernel mm swapper_pg_dir

/* swapper_pg_dir用于存放内核PGD页表的地方，赋给init_mm.pgd */

kernek/head.S
	.globl	swapper_pg_dir
	.equ	swapper_pg_dir, KERNEL_RAM_VADDR - PG_DIR_SIZE

mm/init-mm.c

struct mm_struct init_mm = {
  .mm_rb    = RB_ROOT,
  .pgd    = swapper_pg_dir,
  .mm_users  = ATOMIC_INIT(2),
  .mm_count  = ATOMIC_INIT(1),
  .mmap_sem  = __RWSEM_INITIALIZER(init_mm.mmap_sem),
  .page_table_lock =  __SPIN_LOCK_UNLOCKED(init_mm.page_table_lock),
  .mmlist    = LIST_HEAD_INIT(init_mm.mmlist),
  .user_ns  = &init_user_ns,
  INIT_MM_CONTEXT(init_mm)
};



//TODO - kernel fixmap

/* http://www.wowotech.net/memory_management/fixmap.html */

* 虚拟地址固定 物理地址不固定
* 内存模块初始化之前 已经可以建立临时固定映射 就算内存模块初始化完成后 固定映射的虚拟地址任然不变

//TODO - kernel struct task_struct stack

/* https://ty-chen.github.io/linux-kernel-task-struct/ */


//TODO - kernel 构建页表 set_pte

void set_pte_at(struct mm_struct *mm, unsigned long addr, pte_t *ptep, pte_t pteval);
     set_pte_at(&init_mm,             va,                 pg,          pfn_pte(pa >> PAGE_SHIFT, __pgprot(flags)));


//TODO - kernel 线性 映射

struct page *page;
page = alloc_pages(gfp_mask & ~__GFP_HIGHMEM, order);
page_address(page)
	if (!PageHighMem(page))
		return lowmem_page_address(page);
			return page_to_virt(page);
					pfn_to_virt(page_to_pfn(page))
						return __va(pfn) << PAGE_SHIFT;

// TODO - [struct page 分类]
* 根据struct page中的union分类:
	pagecache和anonymous
	slab
	compound page
	Page table
	hugetlb
	ZONE_DEVICE pages

//TODO - kernel WRITE_ONCE READ_ONCE

74c91db9ec95ffd89849996b8f1c52af
READ_ONCE()
WRITE_ONCE()

//TODO - kernel list.h struct list_head

list_splice()
https://linux.laoqinren.net/kernel/list/



//TODO - windows get cpu.bat

adb root

set dd=%DATE:~8,2%
set mm=%DATE:~5,2%
set yy=%DATE:~0,4%
set Tss=%TIME:~6,2%
set Tmm=%TIME:~3,2%
set Thh=%TIME:~0,2%
set Thh=%Thh: =0%

set currenttime=%yy%%mm%%dd%%Thh%%Tmm%%Tss%

set save_file=cpu_result_%currenttime%.txt

:get_cpu
adb shell date >> %save_file%
adb shell COLUMNS=512 top -b -m 100 -d 2 -n 1 -b >> %save_file%
goto get_cpu

pause


//TODO - windows get mm.bat

adb root

set dd=%DATE:~8,2%
set mm=%DATE:~5,2%
set yy=%DATE:~0,4%
set Tss=%TIME:~6,2%
set Tmm=%TIME:~3,2%
set Thh=%TIME:~0,2%
set Thh=%Thh: =0%

set currenttime=%yy%%mm%%dd%%Thh%%Tmm%%Tss%

set save_file=mem_result_%currenttime%.txt

for /l %%i in (1,1,1000000) do (
adb shell date >> %save_file%
adb shell dumpsys -t 100 meminfo >> %save_file%
adb shell date >> %save_file%
adb shell cat /proc/meminfo >> %save_file%

timeout /t 1
)

//TODO - windows get log.bat

@echo off
adb root

md d:\log\anr
md d:\log\log
md d:\log\tcpdump
md d:\log\tombstones
md d:\log\recovery
md d:\log\dropbox
md d:\log\databases

adb pull /data/anr/ d:\log\anr
adb pull /data/log/ d:\log\log
adb pull /data/tcpdump/ d:\log\tcpdump
adb pull /data/tombstones/ d:\log\tombstones
adb pull /cache/recovery  d:\log\recovery
adb pull /data/system/dropbox  d:\log\dropbox
adb pull /data/data/com.android.providers.media/databases/  d:\log\databases

pause

//TODO - windows enable touchpointer.bat

adb root

adb shell settings put system show_touches 1
adb shell settings put system pointer_location 1

pause


//TODO - bash 2>&1

command > output.txt 2>&1		// stderr和stdout全部输出到output.txt


//TODO - bash timestamp

echo "$(date '+%Y-%m-%d %H:%M:%S') - Melo"

//TODO - kernel idr

https://cloud.tencent.com/developer/article/1432802


//TODO - kernel wait_queue

等待队列分两步
1 为了使得等待进程在一个等待队列中睡眠，需要调用函数wait_event()函数。进程进入睡眠，将控制权释放给调度器。
2 在内核中另一处，调用wake_up()函数唤醒等待队列中的睡眠进程。


queue 是等待队列头，condition 是条件，
如果调用 wait_event 前 condition == 0 ，则调用 wait_event 之后，当前进程就会休眠。


wait_event：将当前进程的状态设置为   TASK_UNINTERRUPTIBLE  然后 schedule()
wait_event_interruptible：         TASK_INTERRUPTIBLE    然后 schedule()
wait_event_timeout：               TASK_UNINTERRUPTIBLE  然后 schedule_timeout()
wait_event_interruptible_timeout:  TASK_INTERRUPTIBLE    然后 schedule_timeout()


别的进程发来一个信号比如 kill ，
TASK_INTERRUPTIBLE 就会醒来去处理。
TASK_UNINTERRUPTIBLE 不会
kernel str 有kill -9 all的行为


schedule()，进程调度，而schedule_timeout()进行调度之后，一定时间后自动唤醒。


比如你调用 wake_up 去唤醒一个使用 wait_event 等，进入休眠的进程
唤醒之后，它会判断 condition 是否为真，
如果还是假的继续睡眠

成功地唤醒一个被wait_event_interruptible()的进程，需要满足：
1 condition为真的前提下
2 调用wake_up()


//TODO - kernel KSM Kernel Samepage Merging

如果发现完全相同的内存页就会合并为单一内存页，并标志位写时复制COW(Copy On Write)



//TODO - 火焰图

/* 记录系统/进程的堆栈采样数据，默认perf.data */
perf record -g –p

/* 对record的调用栈等信息进行解析，默认perf.unflod */
perf script，

将perf.unflod导入PC机，使用火焰图工具出解析perf.svg。

FlameGraph/stackcollapse-perf.pl perf.unflod FlameGraph/flamegraph.pl > perf.svg。

使用chrome浏览器打开。



//TODO - kernel lru

LRU_INACTIVE_ANON：不活跃匿名页链表。
LRU_ACTIVE_ANON：活跃匿名页链表。
LRU_INACTIVE_FILE：不活跃文件页链表。
LRU_ACTIVE_FILE：活跃文件页链表。
LRU_UNEVICTABLE：不可回收页链表。