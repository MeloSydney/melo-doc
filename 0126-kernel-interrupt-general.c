
//TODO - linux 中断向量表 地址

0000:0000   0000:03FF的1024个单元


//TODO - linux 中断过程

1 获取中断类型
2 标志寄存器入栈
3 设置标志寄存器的tf和if
4 cs入栈
5 ip入栈
6 中断类型码*4 和 中断类型码*4+2的两个空间获取中断程序入口 设置cs和ip


//TODO - kernel GIC

PPI 私有外设中断
SPI 共享外设中断
SGI 软件产生中断 用户可编程
LPI locality-specific peripheral interrupt MSI/MSI-X
IRI interrupt routing infrastructure
ITS interrupt translation service 翻译message-base interrupt成LPI
IRR interrupt request register pending状态的中断
ISR interrupt service routing CPU可见
EOI end-of-interrupt
TPR task-priority register 如果小于TPR CPU不处理
PPR processor-priority register 当前CPU正在处理的中断优先级
TMR trigger mode register 中断触发方式





