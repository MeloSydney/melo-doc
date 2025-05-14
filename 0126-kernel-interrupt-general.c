
// TODO - [gic 中断向量表 地址 x86]

0000:0000   0000:03FF的1024个单元


// TODO - [gic 中断过程]

1 获取中断类型
2 标志寄存器入栈
3 设置标志寄存器的tf和if
4 cs入栈
5 ip入栈
6 中断类型码*4 和 中断类型码*4+2的两个空间获取中断程序入口 设置cs和ip


// TODO - gic 关键名词

/*
 * gic 四种中断类型
 */
SGI 0-15 软件产生中断 software Generate Interrupt 通过写GICD_SGIR寄存器来触发一个中断事件 [类似 X86 IPI中断]
PPI 16-31 私有外设中断 [类似 x86上的LAPIC Timer Interrupt]
SPI 32-1019 共享外设中断 [GICD_SETSPI_NSR设置中断，GICD_CLRSPI_NSR清除中断]
LPI 8192+ [Locality-specific Peripheral Interrupt e.g. MSI/MSI-X]	[通过ITS翻译]

/*
 * gic 四大组件
 */
GICC_	[Cpu interface 位于cpu侧 msr]
GICD_	[Distributor]
GICR_	[Redistributor]
GICH_	[Virtual interface控制寄存器 Hyp 访问]
GICV_	[Virtual cpu interface]
GITS_	[Interrupt Translation Service]
ICC_	[物理 cpu interface 系统寄存器]
ICV_	[虚拟 cpu interface 系统寄存器]
ICH_	[虚拟 cpu interface 控制系统寄存器]

refs: https://blog.csdn.net/xy010902100449/article/details/122899831

IRI interrupt routing infrastructure
ITS interrupt translation service 翻译message-base interrupt成LPI
IRR interrupt request register pending状态的中断
ISR interrupt service routing CPU可见
EOI end-of-interrupt
TPR task-priority register 如果小于TPR CPU不处理
PPR processor-priority register 当前CPU正在处理的中断优先级
TMR trigger mode register 中断触发方式





// TODO - [gic 声明周期]

1 Start
2 Generate	[外设产生中断]
3 Distribute
4 Deliver	[将中断发送给PE]
5 Activate	[PE响应中断]
6 Priority Drop	[PE完成中断响应]
7 Deactivate
8 End


// TODO - [gic 中断分组]

Group 0			[EL3处理]
secure Group 1		[secure EL1或secure EL2处理]
no-secure Group 1	[non secure EL1或non secure EL2处理]

// TODO - [gic 寄存器]

/*
 * GICD 前缀 通过memory-mmaped访问 	[gic内部 cpu访问更加频繁 指令: msr]
 * ICC  前缀 直接访问寄存器 		[gic外部]
 * ICC  物理 cpu interface 系统寄存器 msr
 * ICV  虚拟 cpu interface 系统寄存器 msr
 * ICH  虚拟 cpu interface 控制系统寄存器
 */
GICD_ISENABLER		[set enable]
GICD_ICENABLER		[clear enable]
GICD_ICFGR		[中断触发方式 00: 电平触发; 01: 边沿触发;]
GICD_IPRIORITYR		[优先级]
GICD_CTLR		[ARE_S ARE_NS(no-secure) 亲和性]
			[DS Disable Secure]
			[EnableGrp1S  使能Secure Group 1    中断的分发]
			[EnableGrp1NS 使能Non-secure Group 1中断的分发]
			[EnableGrp0   使能Non-secure Group 0中断的分发]
GICD_IROUTERx		[Interrupt_Routing_Mode 31 IRM==0 指定PE 由Affx配置; IRM==1 1toN;]
			[Aff3 39:32]
			[Aff2 23:16]
			[Aff1 15:8]
			[Aff0 7:0]
GICD_IGROUPR		[SPI 中断分组]
GICD_IGRPMODR		[SPI 中断分组]
GICD_ISPENDR
GICD_SETSPI		[SPI 声明]
GICD_CLRSPI		[SPI 清除]
GICD_ISACTIVERx		[SPI set active]
GICD_ICACTIVERx		[SPI clear active]
/*
 * GICR_
 */
GICR_IPRIORITYn		[优先级]
GICR_ICFGRn		[触发方式]
GICR_ISENABLER		[set enable]
GICR_ICENABLER		[clear enable]
GICR_CTLR
GICR_ISACTIVER0		[SPI set active 中断(0-31)]
GICR_ICACTIVER0		[SPI clear active 中断(0-31)]
GICR_IGROUPR0		[PPI SGI 中断分组]
GICR_IGRPMODR0		[PPI SGI 中断分组]
GICR_NSACR
GICR_IPRIORITYRx
GICR_SETLPIR		[LPI Set]
GICR_CLRLPIR		[LPI Clear]
GICR_WAKER		[ProcessorSleep =0 PE唤醒; =1 PE quiescent]
			[ChildrenAsleep =0 PE非低功耗状态; =1 PE低功耗状态]

/*
 * SCR_
 */
SCR_ELx			[armv8 中断配置]
/*
 * ICC_
 */
ICC_SGI0R_EL1		[SGI 产生Secure Group 0中断]
ICC_SGI1R_EL1		[SGI 为PE当前安全状态 产生Group 1中断]
ICC_ASGI1R_EL1		[为PE的其它安全状态 产生Group 1中断]
			[SGI ID 27:24 软中断号]
			[Interrupt Routing Mode:IMR 40
				IRM=0: 路由<aff3>.<aff2>.<aff1>.<Target List>
				IMR=1: 路由除了产生中断的PE之外的所有连接的PE上]
			[Affinity3 55:48]
			[Affinity2 39:32]
			[Affinity1 23:16]
			[Target List 15:0]
ICC_EOIR0_EL1		[End Of Interrupt Register 结束中断处理 允许调度下一次中断 只执行优先级Drop]
ICC_EOIR1_EL1
ICC_IAR0_EL1		[Interrupt Acknowledge Register 应答Group 0中断 并获取中断号]
ICC_IAR1_EL1
ICC_DIR_EL1		[Deactivate Interrupt Register 显式地将中断标记为 Inactive]
			[INTID 23:0 Deactivated中断ID 只有EOImode=1时 才需要]
ICC_CTLR_EL1		[EOImode EOImode=1: 只执行优先级降落; EOImode=0: 将该物理中断失效]
			[PRI PE的优先级]
ICC_BPR0_EL1		[优先分组]
ICC_BPR1_EL1		[优先分组]
ICC_PMR_EL1		[小于该值的中断才能被发送给PE]
ICC_RPR_EL1		[Priority 7:0 当前PE上处于active状态中断的组优先级]
ICC_AP0Rx_EL1		[IMPLEMENTATION DEFINED]
ICC_IGRPEN0_EL		[Enable =1 Enable Group 0]
ICC_IGRPEN1_EL1		[EnableGrp1S =1 Enable Group 1 Secure]
			[EnableGrp1NS =1 Enable Group 1 No Secure]
ICC_HPPIR0		[INTID 23:0 当前pending状态的最高优先级的INTID]
ICC_HPPIR1
ICC_SRE_EL1		[Interrupt Controller System Register Enable Register 寄存器 or MMAP访问]
/*
 * ICH_
 */
ICH_ELRSR_EL2		[寻找空闲的List Register]
ICH_LRx_EL2		[List Register HW=1 虚拟中断是与物理中断关联;]
ICH_AP0R<n>_EL2		[Group 0 优先级]
ICH_AP1R0_EL2		[当前PE处理的中断的优先级 用于高优先级抢占]
ICH_AP1R1_EL2
ICH_AP1R2_EL2
ICH_AP1R3_EL2
ICH_VMCR_EL2 		[VM中断控制寄存器]
ICH_HCR_EL2		[Interrupt Controller Hyp Control Register 虚拟化中断控制寄存器]
/*
 * GITS_
 */
GITS_TRANSLATER		[产生LPI中断]


// TODO - [gic 安全等级]


Secure Group 0
	EL3中断 安全固件
	FIQ形式发送

Secure Group 1
	Secure EL1中断 Trusted OS

Non-secure Group 1
	非安全状态的中断 OS和/或hypervisor


// TODO - [gic Affinity]

情和性
	msr MPDIR_EL1


// TODO - [gic SDEI]

SDEI
	Software Delegated Exception Interface
	属于 Instruction Abort 异常

为何使用
	为使VM单独重启与中断直通并存
	需要有一种机制来支持在中断直通的情况下
	可以发送核间中断到el2 这就是需要支持SDEI的原因



// TODO - [gic Distributor]

Distributor GICD_*

可配置
	1 SPI中断优先级和分发
	2 使能、禁止SPI中断
	3 配置各个SPI中断的优先级
	4 每个SPI中断的路由信息
	5 配置各个SPI中断的触发方式 电平或边沿
	6 产生基于消息的SPI中断
	7 控制SPI中断的active和pending状态
	8 确定各个安全状态中使用的编程模型（亲和力路由还是旧方式）


// TODO - [gic Redistributor]

Redistributor GICR_*
	Redistributor掌握着所有物理LPI中断的控制 优先级和挂起状态信息

可配置
	1 使能、禁止SGI和PPI中断
	2 配置SGI和PPI中断的优先级
	3 配置PPI中断的触发方式 电平或边沿
	4 给每一个SGI和PPI中断分配一个中断组
	5 控制SGI和PPI中断的状态
	6 控制LPI中断属性和挂起状态的数据结构在内存中的基地址
	7 每个PE的电源管理


// TODO - [gic Cpu Interface]

CPU Interface ICC_*_ELn

虚拟控制寄存器 ICH_*_EL2

虚拟CPU接口 ICV_*_ELn [VM内部使用 格式/功能和ICC_*_ELn相同]

可配置
	1 使能中断处理的控制和配置
	2 应答中断
	3 去掉中断的优先级并失效中断
	4 为PE配置中断优先级掩码
	5 为PE配置抢占策略
	6 为PE确定最高优先级的挂起中断


// TODO - [gic List Register]

作用
	向vPE中注入虚拟中断使用 [相当于所有active pending中断的cache]
	两种状态: Inused Unused
	包含信息: 虚拟中断号 优先级 中断状态



// TODO - [gic ITS]

作用
	解析LPI