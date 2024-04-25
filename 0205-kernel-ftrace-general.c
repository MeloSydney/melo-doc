

//TODO - ftrace kernel CONFIG

-pg  /* 添加 mcount() */

CONFIG_DYNAMIC_FTRACE    /* mcount() 替换 NOP */
CONFIG_FTRACE=y
CONFIG_STACK_TRACER=y
CONFIG_FUNCTION_TRACER=y
CONFIG_FUNCTION_GRAPH_TRACER=y
CONFIG_HAVE_DYNAMIC_FTRACE=y
CONFIG_HAVE_FUNCTION_TRACER=y
CONFIG_IRQSOFF_TRACER=y
CONFIG_SCHED_TRACER=y
CONFIG_FTRACE_SYSCALLS=y
CONFIG_PREEMPTIRQ_EVENTS=y
CONFIG_TRACER_SNAPSHOT=y

//TODO - ftrace 文件节点

buffer_size_kb     /* percpu ringbuffer 大小 */
buffer_total_size_kb /* 总共 ringbuffer 大小 */
trace_options      /* 可以设置 输出格式 和过滤规则 */
available_tracers  /* cat available_tracers   function_graph function sched_switch nop */
current_tracer     /* echo function > current_tracer */
	/* echo function_graph > current_tracer
	   echo __do_fault > set_graph_function //跟踪__do_fault */
set_ftrace_notrace /* 设置 不想追踪的函数 */
	/* echo schedule > set_ftrace_notrace */
tracing_on         /* ftrace 开关 */
set_ftrace_pid     /* echo $PID > set_ftrace_pid */
trace_marke        /* echo hello world > trace_marke */  /* userspace 通过这种方式 可以记录一条trace */
		   /* https://lwn.net/Articles/366796/ trace_marke userspace 使用 demo */
cat per_cpu/cpu0/trace  /* per_cpu */
events
	enabled
	filter  /* 过滤 */
	format  /* 输出格式 */
kprobe_events      /* krpobe 激活 */
kprobe_profile     /* kprobe 输出 */
max_graph_depth    /* 调用栈最大深度 func_graph 专用 */
set_event          /* 特定事件触发时 打开 ftrace */
set_event_pid      /* 追踪 pid */
set_ftrace_filter  /* 白名单 */		/* CONFIG_DYNAMIC_FTRACE 动态ftrace */
set_ftrace_notrace /* 黑名单 */		/* CONFIG_DYNAMIC_FTRACE 动态ftrace */
set_graph_function /* func-trace 白名单 */	/* CONFIG_DYNAMIC_FTRACE 动态ftrace */
trace_clock        /* https://zhuanlan.zhihu.com/p/611163404 */
options
	func_stack_trace  /* echo 1 > options/func_stack_trace 查看调用栈 function tracer 专用 */
trace_maker        /* 支持 us 打印日志 */
	/* https://www.cnblogs.com/Linux-tech/p/14110332.html */

trace_printk("read foo %d out of bar %p\n", bar->foo, bar);

https://github.com/x-lugoo/linux/tree/linux-objtrace-v6


//TODO - ftrace 挂载 mount

tracefs  /sys/kernel/tracing  tracefs defaults  0  0  /* ftrace mount */
mount -t tracefs nodev /sys/kernel/tracing  /* mount */
ln -s /sys/kernel/tracing /tracing

//TODO - ftrace ringbuffer


//TODO - ftrace debugfs

cat trace_pipe > /tmp/trace.out &  /* 后台记录 */

//TODO - ftrace tracer

函数类：function， function_graph， stack
延时类：irqsoff， preemptoff， preemptirqsoff， wakeup， wakeup_rt， waktup_dl
其他类：nop， mmiotrace， blk



//TODO - ftrace event filter

/sys/kernel/debug/tracing/events/signal/signal_generate/format
root@e05-e371-b1-2:/sys/kernel/debug/tracing# cat events/signal/signal_generate/format
name: signal_generate
ID: 158
format:
        field:unsigned short common_type;       offset:0;       size:2; signed:0;
        field:unsigned char common_flags;       offset:2;       size:1; signed:0;
        field:unsigned char common_preempt_count;       offset:3;       size:1; signed:0;
        field:int common_pid;   offset:4;       size:4; signed:1;

        field:int sig;  offset:8;       size:4; signed:1;
        field:int errno;        offset:12;      size:4; signed:1;
        field:int code; offset:16;      size:4; signed:1;
        field:char comm[16];    offset:20;      size:16;        signed:1;
        field:pid_t pid;        offset:36;      size:4; signed:1;
        field:int group;        offset:40;      size:4; signed:1;
        field:int result;       offset:44;      size:4; signed:1;

print fmt: "sig=%d errno=%d code=%d comm=%s pid=%d grp=%d res=%d", REC->sig, REC->errno, REC->code, REC->comm, REC->pid, REC->group, REC->result

/* 可以依赖 field 设置 filter */

/* 根据 进程名 comm 过滤 */
echo 'comm == "rcu_sched"' > 	/sys/kernel/debug/tracing/events/sched/sched_wakeup/filter
echo 1 > 			/sys/kernel/debug/tracing/events/sched/sched_wakeup/enable
cat 				/sys/kernel/debug/tracing/trace

/* 根据 pid 过滤 */
echo 'pid > 1000' > 		/sys/kernel/debug/tracing/events/sched/sched_wakeup/filter
echo 1  > 			/sys/kernel/debug/tracing/events/sched/sched_wakeup/enable
cat 				/sys/kernel/debug/tracing/trace


//TODO - ftrace event triggers

/* 做什么[:几次] [if 条件] */
echo 'command[:count] [if filter]' > events/trigger

/* if sys_enter_read 触发 do enable_event  kmem:kmalloc 1次 */
echo 'enable_event:kmem:kmalloc:1' > /sys/kernel/tracing/events/syscalls/sys_enter_read/trigger
/* if sys_exit_read 触发  do disable_event kmem:kmalloc */
echo 'disable_event:kmem:kmalloc' > /sys/kernel/tracing/events/syscalls/sys_exit_read/trigger
/* if sys_read 触发 & if bytes_req == 256 do enable_event kmem:kmalloc 1次 */
echo 'enable_event:kmem:kmalloc:1 if bytes_req == 256' >
	/sys/kernel/tracing/events/syscalls/sys_enter_read/trigger

echo "common_pid > 1000" >
	/sys/kernel/tracing/events/syscalls/sys_enter_read/trigger

/* if kmalloc 触发 do stacktrace */
echo 'stacktrace' > /sys/kernel/debug/tracing/events/kmem/kmalloc/trigger
/* if kmalloc 触发 & if bytes_req >= 800   do stacktrace 2次 */
echo 'stacktrace:2 if bytes_req >= 800' > /sys/kernel/debug/tracing/events/kmem/kmalloc/trigger
/* if kmalloc 触发 & if bytes_req >= 65536 do stacktrace 5从 */
echo 'stacktrace:5 if bytes_req >= 65536' > /sys/kernel/tracing/events/kmem/kmalloc/trigger
/* if kmalloc 触发 & if bytes_req >= 65536 do snapshot 1次 */
echo 'snapshot:1 if bytes_req >= 65536' > events/kmem/kmalloc/trigger

command
	enable_event/disable_event
		/* 当触发sysread时 kmalloc被触发 1次 */
		/* enable_event:<system>:<event>[:count] */
		echo 'enable_event:kmem:kmalloc:1' > \
			/sys/kernel/tracing/events/syscalls/sys_enter_read/trigger

	stacktrace
		/* 当kmalloc触发时 触发stacktrace */
		echo 'stacktrace' > \
			/sys/kernel/tracing/events/kmem/kmalloc/trigger
		/* 当kmalloc触发时 并且bytes_req >= 65535 触发stacktrace */
		echo 'stacktrace:5 if bytes_req >= 65536' > \
			/sys/kernel/tracing/events/kmem/kmalloc/trigger

	snapshot
		/* 当block_unplug触发 并且nr_rq > 1 触发snapshot 1次 */
		echo 'snapshot:1 if nr_rq > 1' > \
			/sys/kernel/tracing/events/block/block_unplug/trigger

	traceon/traceoff
		/* 当block_unplug触发 并且nr_rq > 1 触发traceoff 1次 */
		echo 'traceoff:1 if nr_rq > 1' > \
			/sys/kernel/tracing/events/block/block_unplug/trigger





//TODO - ftrace start_ftrace.sh

#!/bin/bash
#mount -t debugfs nodev /sys/kernel/debug
echo nop > /sys/kernel/debug/tracing/current_tracer
echo 0 > /sys/kernel/debug/tracing/tracing_on
echo   > /sys/kernel/debug/tracing/trace
#echo "function" > /sys/kernel/debug/tracing/current_tracer
echo "sched_switch sched_wakeup
	sched_wakeup_new sched_migrate_task irq_handler_entry
	irq_handler_exit softirq_raise softirq_entry softirq_exit" > /sys/kernel/debug/tracing/set_event
echo "workqueue_execute_start workqueue_execute_end
	block_rq_issue block_rq_insert block_rq_complete" >> /sys/kernel/debug/tracing/set_event
echo 20480 > /sys/kernel/debug/tracing/buffer_size_kb
echo 1 > /sys/kernel/debug/tracing/tracing_on
echo "frace start!"


#!/bin/bash
echo "stop ftrace!"
echo 0 > /sys/kernel/debug/tracing/tracing_on
cat /sys/kernel/debug/tracing/trace > ./ftrace.log
#echo nop > /sys/kernel/debug/tracing/current_tracer
echo > /sys/kernel/debug/tracing/trace




//TODO - ftrace 跟踪 kill xxx 命令

sh -c "echo $$ > set_ftrace_pid; echo 1 > tracing_on; kill xxx; echo 0 > tracing_on"



//TODO - ftrace irqoff

echo 0 > options/function-trace
echo irqsoff > current_tracer
echo 1 > tracing_on
echo 0 > tracing_max_latency
 [...]
echo 0 > tracing_on
cat trace


# tracer: irqsoff
#
# irqsoff latency trace v1.1.5 on 4.0.0+
# --------------------------------------------------------------------
# latency: 11658 us, #4/4, CPU#2 | (M:server VP:0, KP:0, SP:0 HP:0 #P:4)
#    -----------------
#    | task: swapper/2-0 (uid:0 nice:0 policy:0 rt_prio:0)
#    -----------------
#  => started at: rcu_idle_enter
#  => ended at:   arch_cpu_idle---------------------记录禁止中断时间最长的开始和结束函数
#
#
#                  _------=> CPU#
#                 / _-----=> irqs-off----------------d表示中断被disabled，'.'表示中断没有被关闭。
#                | / _----=> need-resched------------N-表示need_resched被设置；'.'-表示need_resched没有被设置，中断返回不会进行进程切换。
#                || / _---=> hardirq/softirq---------H-表示softirq中发生了硬件中断；h-硬件中断；s-softirq；'.'-不在中断上下文中。
#                ||| / _--=> preempt-depth-----------当抢占中断势能后，该域代表preempt_disabled的级别。
#                |||| /     delay
#  cmd     pid   ||||| time  |   caller      ---------cmd-进程名，pid-进程id，time-表示trace从开始到当前的相对时间，delay-突出显示那些有高延迟的地方以便引起注意。！表示需要引起注意。
#     \   /      |||||  \    |   /
  <idle>-0       2d...    3us#: rcu_idle_enter
  <idle>-0       2d... 11645us+: arch_cpu_idle
  <idle>-0       2d... 11665us+: trace_hardirqs_on <-arch_cpu_idle
  <idle>-0       2d... 11753us : <stack trace>
 => cpu_startup_entry
 => secondary_start_kernel




//TODO - ftrace good doc

/* https://www.cnblogs.com/arnoldlu/p/7211249.html */

/* https://hotttao.github.io/posts/linux/linux_perf/03_ftrace/ */



//TODO - ftrace trace_printk
fs_initcall(init_trace_printk_function_export);
	ret = tracing_init_dentry();
	trace_create_file("printk_formats", 0444, NULL, NULL, &ftrace_formats_fops);
		ret = tracefs_create_file(name, mode, parent, data, fops);
			inode = tracefs_get_inode(dentry->d_sb);
			d_instantiate(dentry, inode);
			fsnotify_create(dentry->d_parent->d_inode, dentry);



//TODO - ftrace 开机 使能
trace_event=sched:sched_process_fork,irq:,thermal


//TODO - ftrace kprobe_events

echo 'p bio_add_page arg1=$arg1 arg2=$arg2' > ./kprobe_events
cat events/kprobes/p_bio_add_page_0/trigger
echo 1 > events/kprobes/p_bio_add_page_0/enable
...
cat trace

sudo echo 'p:my_grp/arm64_sys_openat __arm64_sys_openat dfd=$arg1 flags=$arg3 mode=$arg4' >> kprobe_events
sudo echo 'r:my_grp/arm64_sys_openat __arm64_sys_openat ret=$retval' >> kprobe_events



//TODO - ftrace objtrace

echo 'objtrace:add:arg1,0x28:u32:5 if comm == "cat"' > ./events/kprobes/p_bio_add_page_0/trigger


//TODO - ftrace uprobe


readelf -s /bin/bash | grep -w readline
	920: 00000000000d6070   208 FUNC    GLOBAL DEFAULT   13 readline  /* 查看 readline 符号的地址 */

echo 'p:my_grp/readline /bin/bash:0xd6070' >> uprobe_events  /* 追踪 readline */
echo 1 > events/my_grp/readline/enable
echo 0 > events/my_grp/readline/enable
echo '-:my_grp/readline' >> uprobe_events