

//TODO - [perf general doc]

https://www.brendangregg.com/perf.html
https://segmentfault.com/a/1190000021465563?utm_source=tag-newest


// TODO - [perf 行为分类]

[Counting Events perf stat]

[Profiling perf record -F]

[Static Tracing perf record -e ]

[Dynamic Tracing perf probe]

[Reporting perf report / perf script]

// TODO - [perf general 数据分类]

[hardware event PMU硬件产生] [software event 内核软件产生] [tracepoint event 内核静态tp]


//TODO - [perf record]

[-F 采样频率]
perf record -F 99 command

[-p PID过滤]
perf record -F 99 -p PID

[-g graph调用关系]
perf record -e cpu-clock -g -p 11110 -o data/perf.data sleep 30

[--call-graph 调用栈回溯方式 fp|dwarf|lbr]

[-a 所有cpu]
perf record -g -a -F 99  sleep 6

[-t 过滤线程]

[-o 输出文件]

[-c 采样周期]

[-p PID过滤]

[-R raw samples]

[-e 子系统:符号]
perf record -e block:block_rq_issue -ag


//TODO - [perf report]

perf record -e cpu-clock ./t1  /* e 具体event */

[--stdio 输出控制台]
perf report --stdio


//TODO - [perf list]

perf list

[sched:* 子系统中所有符号]
perf list 'sched:*'



//TODO - [perf top]

[不知道哪个程序 针对整个系统]

[:u 针对用户态 ,cycles 多个采样]
perf top -e branch-misses:u,cycles

[{} 多个事件采样]
perf top -e '{branch-misses,cycles}:u'

[-s 输出列排序分类]
perf top -e 'cycles' -s comm,pid,dso

[-K 隐藏kernelspace]

[-U 隐藏userspace]

//TODO - [perf stat]

[针对某个程序专门使用 例如 perf stat ls]

perf stat -e raw_syscalls:sys_enter ls
perf report

perf stat -B dd if=/dev/zero of=/dev/null count=1000000
	-B 千位数之间用逗号分隔符
perf stat -e sched:sched_switch --filter 'prev_pid == 4794' -A -I 1000
	pid 4794
	event sched:sched_switch
	-a all cpus
	-I 1000 打印时间间隔1s
perf stat -e cycles:u dd if=/dev/zero of=/dev/null count=100000
	:u 只统计userspace
perf stat -B -e cycles:u,instructions:u -a -C 0,3-4 sleep 5
	-C 限制cpu

/* 查看 ls 有执行多少个 syscall sys_enter */

// TODO - [perf tracepoint]

[/sys/kernel/debug/tracing/events/raw_syscalls/sys_enter 一个静态的traceevent]

perf stat -e raw_syscalls:sys_enter ls
perf report


perf record -e 'syscalls:sys_enter_*' -a sleep 6

//TODO - [perf probe]

[添加event usleep_range 打印arg1~arg5 但是参数打印不正确]
perf probe 'usleep_range %x1 %x2 %x3 %x4 %x5'


[--filter 添加过滤规则]
perf record -e probe:tcp_sendmsg --filter 'size > 0 && skc_state != 1' -a

[--filter 系统调用号>200]
perf trace record -e 'raw_syscalls:sys_enter' --filter 'id >= 200' sleep 3

[--filter nr_sector > 200]
perf record -e block:block_rq_complete --filter 'nr_sector > 200'

[--filter rwbs == "WS" 字符串匹配]
perf record -e block:block_rq_complete --filter 'rwbs == "WS"'

perf probe schedule:12 cpu

/* 监听 tcp_sendmsg 内核函数 */
perf probe --add tcp_sendmsg
perf record -e probe:tcp_sendmsg -ag -- sleep 5
perf probe --del tcp_sendmsg
perf report

[删除所有]
perf probe -d '*'

[如果有内核符号 可以查看该函数变量]
perf probe --vars do_nanosleep

[没有debuginfo时 可以通过该方法获取变量 x86]
perf probe 'do_nanosleep mode=%si:u32'

/* 在schedule()+12 插入一个动态tp */

//TODO - [perf sched]

perf sched record sleep 10
perf sched latency --sort max  /* 量化调度延迟 */

perf sched record sleep 10
perf sched map  /* 显示所有cpu 上下文切换 */


perf sched record sleep 10
perf sched script  /* 显示 调度相关event */

perf sched replay

//TODO - [perf lock]

perf lock record dd if=/dev/testb_a of=/dev/null
perf lock report  /* 显示 加锁event */

//TODO - [perf kmem]

perf kmem  /* 收集 slub信息 */


// TODO - [perf timechart]

perf timechart

// TODO - [perf ftrace]

[-G graph追踪]

[-t tracer 例如function function_graph]

[-N 不追踪]

[-F 列出所有可追踪函数]

[-D 延迟之后开始]

// TODO - [perf bench]

[内置benchmark]
perf bench mem

[测试benchmark memcpy]
perf bench mem memcpy


// TODO - [perf script]

[对数据进行脚本化处理 用于火焰图后续处理]

// TODO - [perf trace]

[针对系统调用 性能好于strace]


// TODO - [perf annotate]

[用来展示原始明细]


// TODO - [perf 火焰图]

[1 perf record 采样]
$sudo perf record -e cpu-clock -g -p 11110

[2 -i 解析数据]
perf -i perf.data &> perf.unfold

[3 perl脚本处理]
./stackcollapse-perf.pl perf.unfold &> perf.folded

[4 perf脚本 生成火焰图]
./flamegraph.pl perf.folded > perf.svg

