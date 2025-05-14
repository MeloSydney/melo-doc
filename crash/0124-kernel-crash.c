


//TODO - crash help

https://crash-utility.github.io/crash_whitepaper.html#HELP


//TODO - kernel kdump

https://juejin.cn/post/7172962884996530183
runninglinuxkernel

//TODO - kernel makedumpfile


+makedumpfile -c -d 31 /proc/vmcore /kdump/AP-crashdump-$time-$num
+makedumpfile --dump-dmesg /proc/vmcore /kdump/AP-crashdump-$time-$num.dmesg


//TODO - kernel vmlinuz >> vmlinux

extract-vmlinux


//TODO - [kernel Hard Lockup]

Hard Lockup
	长时间关闭cpu中断


crash> p watchdog_thresh   /* 查看watchdog最大容忍s */

watchdog_thresh = $1 = 60  /* 默认10s */

// TODO - [kernel Soft Lockup]

原因
	长时间占有cpu 关闭抢占


watchdog_timer_fn(struct hrtimer *hrtimer)
	unsigned long touch_ts = __this_cpu_read(watchdog_touch_ts);
	duration = is_softlockup(touch_ts);
		if (time_after(now, touch_ts + get_softlockup_thresh()))
			return now - touch_ts;
	/* Soft Lockup 发生了! */
	if (unlikely(duration))


static int get_softlockup_thresh(void)
	return watchdog_thresh * 2;

_setup("watchdog_thresh=", watchdog_thresh_setup);

static int __init watchdog_thresh_setup(char *str)
	get_option(&str, &watchdog_thresh);


//TODO - crash vmcore vmlinux

/* http://www.biscuitos.cn/blog/CRASH/#E01 */
bt -t 			/* 事发cpu的调用栈 */
sym ffff8000109025f8 	/* 解析 符号地址 */
dis dma_buffer_send 	/* 汇编 */
dis -l dma_buffer_send+120 10    /* 汇编 偏移120 解析10行 */
struct scatter_data -o /* 解析 结构体 偏移 */
rd ffff800012145138 32  /* 读取 32B */
rd ffff800012145138 -e ffff800012145148  /* dump的start和end */
p jeffies 		/* 打印变量 */
p mutex_lock 		/* 打印函数地址 */
pd jeffies 		/* 十进制打印 */
px jeffies 		/* 十六进制打印 */
struct task_struct ffff000e7a6fcc00 -x xxxxxx /* 解析 task_struct */
struct mm_struct init_mm  /* 查看 全局变量 init_mm System.map中可以找到 */
struct mm_struct.owner,mm_user ffff000e7a6fcc00  /* 解析owner和mm_user 两个成员 */



//TODO - kernel crash oops 打印

page_fault_oops(struct pt_regs *regs, unsigned long error_code, unsigned long address)
	show_fault_oops(regs, error_code, address);
		pgd_t *pgd;
		pte_t *pte;
		pgd = __va(read_cr3_pa());
		pgd += pgd_index(address);
		pte = lookup_address_in_pgd(pgd, address, &level);
		if (address < PAGE_SIZE && !user_mode(regs))
			pr_alert("BUG: kernel NULL pointer dereference, address: %px\n", (void *)address);
		else
			pr_alert("BUG: unable to handle page fault for address: %px\n", (void *)address);
		dump_pagetable(address);
			pgd_t *base = __va(read_cr3_pa());
			pgd_t *pgd = base + pgd_index(address);
			pr_info("PGD %lx ", pgd_val(*pgd));
			p4d = p4d_offset(pgd, address);
			pr_cont("P4D %lx ", p4d_val(*p4d));
			pud = pud_offset(p4d, address);
			pr_cont("PUD %lx ", pud_val(*pud));
			pmd = pmd_offset(pud, address);
			pr_cont("PMD %lx ", pmd_val(*pmd));
			pte = pte_offset_kernel(pmd, address);
			pr_cont("PTE %lx", pte_val(*pte));
	printk(KERN_DEFAULT "CR2: %016lx\n", address);


//TODO - handle_pte_fault

1. PTE表项无效 并且是文件页 调用do_fault()。
2. PTE表项无效 并且是匿名页 调用do_anonymous_page()。
3. PTE表项有效 但页不在内存 调用do_swap_page()。
4. PTE表项有效 但页不可访问 调用do_numa_page()。
5. PTE表项有效 但试图写一个不可写页 调用do_wp_page()


//TODO - struct task_struct

cur->pid 进程号
cur->comm 进程名
cur->__state 进程状态
cur->exit_state 进程退出的状态
cur->exit_code  进程正常终止的状态码
cur->exit_signal 进程异常终止的信号


//TODO - crash whatis

whatis -o mm_struct

//TODO - crash list

list rwsem_waiter.list -s rwsem_waiter.task,type -h 0xffff800061c57aa0
rwsem_waiter.list 结构体和list成员
-s 输出的成员信息
-h 内嵌 list_head地址


//TODO - crash file

/* 1 获取 struct task */
ccrash> ps
PID    PPID    CPU        TASK           ST  %MEM  VSZ  RSS    COMM
...
177488 64302    1  ffff880436662ab0   RU   0.0  6280  568     ss


/* 2 获取 dentry */
crash> struct file.f_path ffff880436662ab0
  f_path = {
    mnt = 0xffff880432adbe80,
    dentry = 0xffff880101cae5c0
  }


/* 3 获取 path */
crash> files -d 0xffff880101cae5c0
     DENTRY           INODE           SUPERBLK     TYPE      PATH
ffff880101cae5c0 ffff880101c1d598 ffff88043a23e800 REG  /proc/slabinfo