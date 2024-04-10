

### linux wait_queue

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
