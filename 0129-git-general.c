

//TODO - git general


git clone --recurse-submodules https://xxx/xxx


git branch -a  // 查看所有分支
git branch -r // 查看远端分支
git branch --set-upstream-to origin/远程分支名  本地分支名 // 追踪分支
git branch --set-upstream-to=origin/<branch> e05_xen_v2000_project_dev

git checkout -b 本地分支名 origin/远程分支名  // 本地创建分支 并追踪远程分支

git merge  合并分支名 // 分支merge

git push origin 远程仓库名

git fetch --all // 拉取远端所有信息 但是不合入

git reset --hard $commitID  // 回退到某个提交

/* 回退到某个时间点前 */

./repo forall -c "git clean -df && git checkout -f && git reset --hard HEAD~~"; ./repo sync -j 48
./repo forall -c 'commitID=`git log --before "2023-05-08 22:00" -1 --pretty=format:"%H"`; git reset --hard $commitID'


git diff drivers/input/ > getuser.patch

/* 显示mm下面的patch */
tig mm/


git format-patch -1 abcdefg12345 /* 根据hash生成patch */
git format-patch HEAD^   /* 根据上一笔提交  生成patch */
git format-patch -N1  /

git am     /* 打patch的同时 提交commit信息 */

git apply  /* 打patch的同时 不提交commit信息 */
git apply --check 0001-limit-log-function.patch   // 检查 是否有冲突
git apply getuser.patch  // 打patch


patch -p1 < .../my_patch


//TODO - git 乱码

https://www.cnblogs.com/yanzige/p/9810333.html
git config --global i18n.commitencoding utf-8     /* 注释：该命令表示提交命令的时候使用utf-8编码集提交 */
git config --global i18n.logoutputencoding utf-8  /* 注释：该命令表示日志输出时使用utf-8编码集显示 */
export LESSCHARSET=utf-8                          /* 注释：设置LESS字符集为utf-8 */


//TODO - linux armv8 [gic-v3 中断 处理]


SYM_CODE_START_LOCAL_NOALIGN(el1_irq)
	kernel_entry 1				/* 寄存器 保护 */
	el1_interrupt_handler handle_arch_irq
	kernel_exit 1
SYM_CODE_END(el1_irq)


/* handle.c */
set_handle_irq(void (*handle_irq)(struct pt_regs *))
	handle_arch_irq = handle_irq;


set_handle_irq(gic_handle_irq);			/* gic-v3 */


gic_handle_irq(struct pt_regs *regs)
	irqnr = do_read_iar(regs);		/* hwirq */
	if (handle_domain_irq(gic_data.domain, irqnr, regs))
		return __handle_domain_irq(domain, hwirq, true, regs);
			irq = irq_find_mapping(domain, hwirq);          // 逻辑中断号
			generic_handle_irq(irq);
				struct irq_desc *desc = irq_to_desc(irq);
				generic_handle_irq_desc(desc);          // irq desc
					desc->handle_irq(desc);
					/* handle_irq_event(struct irq_desc *desc) */\
						ret = handle_irq_event_percpu(desc);
/* __handle_irq_event_percpu(struct irq_desc *desc, unsigned int *flags) */
retval = __handle_irq_event_percpu(desc, &flags);
	for_each_action_of_desc(desc, action)
		res = action->handler(irq, action->dev_id);	/* 上半部 */
		__irq_wake_thread(desc, action);		/* 下半部 */


/* 中断申请 */
request_threaded_irq(unsigned int irq, irq_handler_t handler,
				irq_handler_t thread_fn, unsigned long irqflags,
				const char *devname, void *dev_id)
	desc = irq_to_desc(irq);
	action = kzalloc(sizeof(struct irqaction), GFP_KERNEL);
	action->handler = handler;
	action->thread_fn = thread_fn;
	retval = __setup_irq(irq, desc, action);
	/* __setup_irq(unsigned int irq, struct irq_desc *desc, struct irqaction *new) */
		ret = setup_irq_thread(new, irq, false);
			struct task_struct *t;
			t = kthread_create(irq_thread, new, "irq/%d-%s", irq, new->name);
