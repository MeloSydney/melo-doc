

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

[提示类]
available_tracers               /* cat available_tracers   function_graph function sched_switch nop */
available_filter_functions      /* 支持的func */
available_events                /* 支持event 列表 */
events
        enabled
        filter                  /* 过滤 */
        format                  /* 输出格式 */

[控制类 通用]
tracing_on                      /* */
tracing_cpumask                 /* cpu mask */
tracing_max_latency             /* */
tracing_thresh                  /* */
current_tracer                  /* echo function > current_tracer */

/sys/kernel/debug/tracing # echo function > current_tracer
[   45.632002] CPU: 0 PID: 111 Comm: sh Not tainted 5.10.0-dirty #35
[   45.632457] Hardware name: linux,dummy-virt (DT)
[   45.632697] Call trace:
[   45.632981]  dump_backtrace+0x0/0x1f8
[   45.633169]  show_stack+0x2c/0x7c
[   45.634039]  ftrace_modify_all_code+0x38/0x118
[   45.634269]  arch_ftrace_update_code+0x10/0x18
[   45.634495]  ftrace_run_update_code+0x2c/0x48
[   45.634727]  ftrace_startup_enable+0x40/0x4c
[   45.634943]  ftrace_startup+0xec/0x11c
[   45.635137]  register_ftrace_function+0x68/0x84
[   45.635369]  function_trace_init+0xa0/0xc4
[   45.635574]  tracer_init+0x28/0x34
[   45.635768]  tracing_set_tracer+0x11c/0x17c
[   45.635982]  tracing_set_trace_write+0x124/0x170
[   45.636224]  vfs_write+0x16c/0x368
[   45.636409]  ksys_write+0x74/0x10c
[   45.636594]  __arm64_sys_write+0x28/0x34
[   45.636923]  el0_svc_common+0xf0/0x174
[   45.637138]  do_el0_svc+0x84/0x90
[   45.637330]  el0_svc+0x1c/0x28
[   45.637510]  el0_sync_handler+0x3c/0xac
[   45.637721]  el0_sync+0x140/0x180

buffer_size_kb                  /* percpu ringbuffer 大小 */
buffer_total_size_kb            /* total ringbuffer 大小 */
trace_options                   /* 可以设置 输出格式 和过滤规则 */
options
        func_stack_trace  /* echo 1 > options/func_stack_trace 查看调用栈 function tracer 专用 */

[func_stack_trace 实用脚本 ./stack_trace.sh sync]
#!/bin/sh
debugfs=/sys/kernel/debug
echo 0          > $debugfs/tracing/tracing_on
echo nop        > $debugfs/tracing/current_tracer
echo function   > $debugfs/tracing/current_tracer
echo $$         > $debugfs/tracing/set_ftrace_pid
echo 1          > $debugfs/tracing/options/func_stack_trace
echo submit_bio > $debugfs/tracing/set_ftrace_filter
echo 1          > $debugfs/tracing/tracing_on
exec "$@"



set_event                       /* 特定事件触发时 打开 ftrace */
set_event_pid                   /* 追踪 pid */

[控制类 func]
function_profile_enabled        /* */
set_ftrace_notrace              /* 设置 不想追踪的函数 echo schedule > set_ftrace_notrace*/
set_ftrace_pid                  /* echo $PID > set_ftrace_pid */
set_ftrace_filter               /* 追踪谁 ? 可以结合filter command */

[echo '__schedule_bug:traceoff:5' > set_ftrace_filter 命中__schedule_bug 5次之后 traceoff]\
[<function>:<command>:<parameter> 格式 command 例如 mod traceon/traceoff snapshot enable_event/disable_event stacktrace snapshot hist]

[控制类 function_graph]
max_graph_depth                 /* 调用栈最大深度 */
set_graph_function              /* 需要显示谁的调用栈; depend on CONFIG_DYNAMIC_FTRACE;
                                echo function_graph > current_tracer;
                                echo __do_fault > set_graph_function */

[set_graph_function 实用脚本]
#!/bin/sh
debugfs=/sys/kernel/debug
echo nop            > $debugfs/tracing/current_tracer
echo 0              > $debugfs/tracing/tracing_on
echo 20             > $debugfs/tracing/max_graph_depth
echo $$             > $debugfs/tracing/set_ftrace_pid
echo function_graph > $debugfs/tracing/current_tracer
echo nvme_queue_rq  > $debugfs/tracing/set_graph_function
echo 1              > $debugfs/tracing/tracing_on
exec "$@"


[输出类]
printk_formats                  /* raw data */
trace                           /* cat trace */
trace_pipe                      /* cat trace_pipe */
snapshot                        /* echo 0 清空释放 / 1 快照 / 2 清空 */
trace_clock                     /* 时钟类型 local global counter https://zhuanlan.zhihu.com/p/611163404 */
trace_stat                      /* percpu 的trace输出 */
per_cpu/                        /* cat per_cpu/cpu0/trace */
enabled_functions               /* */
saved_cmdlines                  /* */
saved_cmdlines_size             /* */
trace_marker                     /* echo hello world > trace_marke
                                https://lwn.net/Articles/366796/ trace_marke userspace 使用 demo */

[未分类]
kprobe_events                   /* 插入dynamic trace events */
kprobe_profile                  /* kprobe 输出 */
instances 			/* 新开创的一个buf 可以独立设置tracer events filter 等 ... */


trace_printk("read foo %d out of bar %p\n", bar->foo, bar);

https://github.com/x-lugoo/linux/tree/linux-objtrace-v6


//TODO - [ftrace mount tracefs]

tracefs  /sys/kernel/tracing  tracefs defaults  0  0  /* ftrace mount */
mount -t tracefs nodev /sys/kernel/tracing  /* mount */
ln -s /sys/kernel/tracing /tracing

//TODO - [ftrace ringbuffer]


//TODO - [ftrace trace_pipe]

cat trace_pipe > /tmp/trace.out &  /* 后台记录 */

//TODO - [ftrace tracer 类型]

[函数类 function function_graph stack]
[延时类 irqsoff preemptoff preemptirqsoff wakeup wakeup_rt waktup_dl]
[其他类 nop mmiotrace blk]



//TODO - [ftrace event filter]

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


//TODO - [ftrace kprobe_events]

echo 'p bio_add_page arg1=$arg1 arg2=$arg2' > ./kprobe_events
cat events/kprobes/p_bio_add_page_0/trigger
echo 1 > events/kprobes/p_bio_add_page_0/enable
...
cat trace

echo 'p:my_grp/arm64_sys_openat __arm64_sys_openat dfd=$arg1 flags=$arg3 mode=$arg4' >> kprobe_events
echo 'r:my_grp/arm64_sys_openat __arm64_sys_openat ret=$retval' >> kprobe_events

[blk_update 事件名 /sys/kernel/debug/tracing/events/kprobes/blk_update]
[blk_update_request 追踪的符号]
[request=$arg1 status=$arg2:u8 bytes=$arg3:u32 打印参数]
echo 'p:blk_update blk_update_request request=$arg1 status=$arg2:u8 bytes=$arg3:u32'

[name=+0x38(+0x8($arg1)):string      path->dentry->d_iname]
[namep=+0(+0x28(+0x8($arg1))):string path->dentry->d_name.name +0表示解引用]
[函数原型 int vfs_open(const struct path *path, struct file *file)]
echo 'p vfs_open+32 name=+0x38(+0x8($arg1)):string namep=+0(+0x28(+0x8($arg1))):string' > ./kprobe_events

echo 'p vfs_open+20 x2=%x2 x4=%x4 x5=%x5' > ./kprobe_events; echo 1 > events/kprobes/enable; cat trace |tail
echo 0 > events/kprobes/enable; perf probe -d '*'
echo 'p vfs_open+36 x2=%x2 x4=%x4 x5=%x5' > ./kprobe_events; echo 1 > events/kprobes/enable; cat trace |tail
echo 0 > events/kprobes/enable; perf probe -d '*'

/* 2737 0000000000002920 <vfs_open>:
   2738     2920:       d503201f        nop
   2739     2924:       d503201f        nop
   2740     2928:       aa0003e3        mov     x3, x0
   2741     292c:       a9bf7bfd        stp     x29, x30, [sp, #-16]!
   2742     2930:       aa0103e0        mov     x0, x1
   2743     2934:       910003fd        mov     x29, sp
   2744     2938:       a9401464        ldp     x4, x5, [x3]
   2745     293c:       a9011424        stp     x4, x5, [x1, #16]
   2746     2940:       d2800002        mov     x2, #0x0                        // #0
   2747     2944:       f9400461        ldr     x1, [x3, #8]
   2748     2948:       f9402021        ldr     x1, [x1, #64]
   2749     294c:       97fff7c5        bl      860 <do_dentry_open>
   2750     2950:       a8c17bfd        ldp     x29, x30, [sp], #16
   2751     2954:       d65f03c0        ret
*/

/* p[:[GRP/][EVENT]] [MOD:]SYM[+offs]|MEMADDR [FETCHARGS]        : Set a probe
   r[MAXACTIVE][:[GRP/][EVENT]] [MOD:]SYM[+0] [FETCHARGS]        : Set a return probe
   p[:[GRP/][EVENT]] [MOD:]SYM[+0]%return [FETCHARGS]            : Set a return probe
   -:[GRP/][EVENT]                                               : Clear a probe

   GRP            : Group name.
   EVENT          : Event name.
   MOD            : Module name which has given SYM.
   SYM[+offs]     : Symbol+offset where the probe is inserted. 例如 vfs_open+20
   SYM%return     : Return address of the symbol
   MEMADDR        : Address where the probe is inserted.
   MAXACTIVE      : Maximum number of instances of the specified function that
                   can be probed simultaneously, or 0 for the default value
                   as defined in Documentation/trace/kprobes.rst section 1.3.1.

   FETCHARGS     : Arguments. Each probe can have up to 128 args.
   %REG          : Fetch register REG 例如 arm64 %x0 %x1 %x2 x86_64 %di %si
   @ADDR         : Fetch memory at ADDR (ADDR should be in kernel)
   @SYM[+|-offs] : Fetch memory at SYM +|- offs (SYM should be a data symbol)
   $stackN       : Fetch Nth entry of stack (N >= 0)
   $stack        : Fetch stack address.
   $argN         : Fetch the Nth function argument. (N >= 1) (\*1) 例如 %arg1 %arg2
   $retval       : Fetch return value.(\*2)
   $comm         : Fetch current task comm.
   +|-[u]OFFS(FETCHARG) : Fetch memory at FETCHARG +|- OFFS address.(\*3)(\*4)
   \IMM          : Store an immediate value to the argument.
   NAME=FETCHARG : Set NAME as the argument name of FETCHARG.
   FETCHARG:TYPE : Set TYPE as the type of FETCHARG. Currently, basic types
                   (u8/u16/u32/u64/s8/s16/s32/s64), hexadecimal types
                   (x8/x16/x32/x64), VFS layer common type(%pd/%pD), "char",
                   "string", "ustring", "symbol", "symstr" and bitfield are
                   supported.

   (\*1) only for the probe on function entry (offs == 0). Note, this argument access
   is best effort, because depending on the argument type, it may be passed on
   the stack. But this only support the arguments via registers.
   (\*2) only for return probe. Note that this is also best effort. Depending on the
   return value type, it might be passed via a pair of registers. But this only
   accesses one register.
   (\*3) this is useful for fetching a field of data structures.
   (\*4) "u" means user-space dereference. See :ref:`user_mem_access`.
 */



[上述命令成功后 会生成一个/sys/kernel/debug/tracing/events/kprobes/p_vfs_open_0]
echo 'stacktrace if name ~ "*test*"' > ./events/kprobes/p_vfs_open_0/trigger

[关于 +offset]
echo 'p vfs_open+32' > ./kprobe_events


//TODO - ftrace objtrace

echo 'objtrace:add:arg1,0x28:u32:5 if comm == "cat"' > ./events/kprobes/p_bio_add_page_0/trigger


//TODO - ftrace uprobe


readelf -s /bin/bash | grep -w readline
        920: 00000000000d6070   208 FUNC    GLOBAL DEFAULT   13 readline  /* 查看 readline 符号的地址 */

echo 'p:my_grp/readline /bin/bash:0xd6070' >> uprobe_events  /* 追踪 readline */
echo 1 > events/my_grp/readline/enable
echo 0 > events/my_grp/readline/enable
echo '-:my_grp/readline' >> uprobe_events