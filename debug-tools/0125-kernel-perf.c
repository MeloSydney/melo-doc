

//TODO - perf doc

https://www.brendangregg.com/perf.html
https://segmentfault.com/a/1190000021465563?utm_source=tag-newest


//TODO - perf record

perf record -F 99 command  /* 抓取 command 99HZ */

perf record -F 99 -p PID  /* 抓取 pid 99HZ */

//TODO - perf list

perf list
perf list 'sched:*'

1 hardware event PMU硬件计数
2 software event 内核软件计数
3 tracepoint event 静态TP


//TODO - perf stat

perf stat ./t1   /* 简单提供程序汇总计数 */



//TODO - perf top

/* 观察 整个系统状态 */


//TODO - perf report

perf record -e cpu-clock ./t1  /* e 具体event */
perf report


//TODO - perf stat

perf stat -e raw_syscalls:sys_enter ls / perf report

/* 查看 ls 有执行多少个 syscall sys_enter */

//TODO - perf probe

perf probe schedule:12 cpu

/* 在schedule()+12 插入一个动态tp */

//TODO - perf sched

perf sched record sleep 10
perf sched latency --sort max  /* 量化调度延迟 */

perf sched record sleep 10
perf sched map  /* 显示所有cpu 上下文切换 */


perf sched record sleep 10
perf sched script  /* 显示 调度相关event */

perf sched replay

//TODO - perf lock

perf lock record dd if=/dev/testb_a of=/dev/null
perf lock report  /* 显示 加锁event */

//TODO - perf kmem

perf kmem  /* 收集 slub信息 */
perf timechart
perf record -e block:block_rq_issue -ag

/* 追踪 block
	-a 全cpu
	-g 生成graph
*/
