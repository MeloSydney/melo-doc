


//TODO - crash help

https://crash-utility.github.io/crash_whitepaper.html#HELP


//TODO - kernel kdump

https://juejin.cn/post/7172962884996530183
runninglinuxkernel

//TODO - kernel makedumpfile


+makedumpfile -c -d 31 /proc/vmcore /kdump/AP-crashdump-$time-$num
+makedumpfile --dump-dmesg /proc/vmcore /kdump/AP-crashdump-$time-$num.dmesg


//TODO - kernel vmlinuz >> vmlinux

extract-vmlinux  kernel源码工程下  或者 /usr/src


//TODO - kernel hardlock

hardlock，一种是**关中断时间过长**，超过了阈值，系统通过NMI发送来

crash> p watchdog_thresh   /* 查看watchdog最大容忍s */

watchdog_thresh = $1 = 60  /* 默认10s */


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