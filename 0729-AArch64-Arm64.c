
//TODO - refs

https://zhuanlan.zhihu.com/p/578252899


//TODO - 寄存器 分类

通用寄存器 General-purpose Registers
x0 - x28	常规数据操作寄存器
x8 - x18	caller-saved
x19 - x29	callee-saved
x29		FP
x30		LR
x31		WZR
X0		存储方法返回值;
X0～X7		方法参数小于8个时; 参数依次在这些寄存器中; 多了就存到栈上;
X8		系统调用的调用号
X9～X15		临时寄存器
X16 X17		通常用作调试或调用目标地址保存寄存器; 在调用外部函数时可作为跳板寄存器;
x18		平台寄存器; 它的使用与平台相关; 尽量不要使用;
x19~x28		临时寄存器; 子程序使用时必须保存;
X29		帧指针寄存器; 也称为 FP[Frame Pointer]; 在函数调用过程中保存当前栈帧的基址;
X30		链接寄存器; 也称为 LR[Link Register]; 在调用函数时; 存储返回地址; 返回时; 直接从 X30 跳回调用点;
SP		特殊寄存器 [SP + 栈使用的空间 = FP]
ZR		零寄存器; 读取时返回 0; 写入时丢弃结果;

PSTATE          状态寄存器 Status Registers

SP_ELx          专用寄存器 Special-purpose Registers
ELR_ELx
SPSR_ELx

SCTLR_ELx       系统寄存器 System Registers
TTBR0_ELx

/*
 * [Memory Attribute Indirection 寄存器]
 * 用于定义不同内存区域的属性; 如缓存策略、访问权限等
 */
MAIR_ELx

/*
 * 转换控制寄存器; 用于控制地址转换的行为和特性; 如地址范围、页大小等
 * https://blog.csdn.net/sinat_32960911/article/details/127856639
 * IPS[34:32] 输出物理地址宽度 [101 48bits,256T 100 52bits,4P]
 * TG1[31:30] 配置页大小[01 16k 10 4k 11 64k]
 * TG0[15:14] 00 4k 01 64k 10 16k
 */
TCR_ELx

/*
 * thread identifying information 存储区域
 * vcpu_id << 0 || dm_id << 8 || logic_cpu_id << 16
 */
TPIDR_EL0
TPIDR_EL1
TPIDR_EL2
TPIDR_EL3

/*
 * EL0 Read-Only Software Thread ID Register
 */
TPIDRRO_EL0

/*
 * [VM id 保存] 用来区分不同的VM
 */
vmpidr_elx

/* mpidr 格式 */
31       30        24 23        16 15         8 7          0
+---------+----------+------------+------------+-------------+
| U       | Aff3     | Aff2       | Aff1       | Aff0        |
+---------+----------+------------+------------+-------------+

/*
 * Multiprocessor Affinity Register [CPU物理ID]
 */
MPIDR_EL1

/*
 * [Physical Address 寄存器] 应该是 输出stage-1的地址 ipa
 */
PAR_EL1

//TODO - armv8 register

/*
 * 当前runtime状态寄存器 PSTATE
 * NZCV[31:28]
 * DAIF[9:6]
 * SS
 * IL
 * EL[3:2] [00 01 10 11 异常等级]
 * nRW[1] [0 aarch64]
 * SP[0] spsel [0 使用sp_el0] [1 使用sp_elx]
 */
PSTATE

/*
 * [Performance Monitors Control 寄存器] 各种技术 溢出 异常之类的
 */
PMCR_EL0

/*
 * [Auxiliary Fault Status 寄存器]
 */
AFSRx_ELx

/*
 * [Auxiliary Memory Attribute Indirection 寄存器]
 */
AMAIR_ELx


/*
 * [Interrupt Controller Hyp Control 寄存器]
 */
ICH_HCR_EL2

/*
 * [Context ID 寄存器]
 */
CONTEXTIDR_ELx

/*
 * [Architectural Feature Access Control 寄存器]
 */
CPACR_EL1

/*
 * [Cache Size Selection 寄存器]
 */
CSSELR_EL1

/*
 * [Thread ID]
 */
TPIDRRO_EL0

/*
 * [Auxiliary Control 用途未知]
 */
ACTLR_EL1

/*
 * 保存 PSTATE
 * SPSR_EL1, SPSR_EL2, SPSR_EL3
 * NZCV[31:28] [负数] [零标志] [进位] [溢出]
 * DAIF[9:6] [断点中断掩码] [Serror掩码] [IRQ掩码] [FIQ掩码]
 * M[4] 运行模式 [0 = aarch64]
 * M[3:2] 异常等级 0b00 = EL0; 0b01 = EL1; 0b10 = EL2; 0b11 = reserved;
 * M[1] 栈指针选择 1 = 选择SP_EL0公共栈; 0 = 选择SP_ELx异常等级专用栈
 * M[0] 目标执行状态 0 = AArch64; 1 = AArch32
 */
SPSR_ELx

/*
 * [Counter-timer Kernel Control Register 控制Timer访问权限]
 */
CNTKCTL_EL1

/*
 * [Virtual Timer控制寄存器]
 */
CNTV_CTL_EL0

/*
 * [存放Virtual Timer的相对值]
 */
CNTV_CVAL_EL0


/*
 * [保存EL1 virtual timer的值]
 */
CNTV_TVAL_EL1


/*
 * [保存EL1 physical timer的值]
 */
CNTP_TVAL_EL0


/*
 * [保存EL1 physical timer]
 */
CNTP_CVAL_EL0


/*
 * [EL1 physical timer控制寄存器]
 */
CNTP_CTL_EL0

/*
 * eret 返回地址
 * ELR_EL1, ELR_EL2, ELR_EL3
 */
ELR_ELx

/*
 * Stack Pointer
 * SP_EL0, SP_EL1, SP_EL2, SP_EL3
 */
SP_ELx

/*
 * Floating-point Control Register
 */
FPCR

/*
 * Floating-point Status Register
 */
FPSR

/*
 * Floating-Point Exception Control Register
 */
FPEXC32_EL2

/*
 * Exception Syndrome Register
 * 保存发生异常时的特征 比如异常分类 ESR_ELx.EC 异常具体原因 ESR_ELx.ISS 访问的字节数 目标寄存器 load还是store
 * https://blog.csdn.net/sinat_32960911/article/details/127856639
 * ISS2[36:32]
 * Exception Class[31:26]
 * Instruction Length[25]
 * Instruction Specific Syndrome[24]    有效位
 * SAS[23:22]                           访问大小
 * Syncdrome Sign Extend[21]            综合扩展签名
 * Syncdrome Register Transfer[20:16]   综合寄存器转移
 * SF[15]                               [1 64bit]
 * AR[14]                               获取/释放
 * VNCR[13]                             异常是否来自VNCR_EL2寄存器
 * SET[12:11]                           同步错误类型
 * FnV[10]                              FAR 地址是无效的
 * EA[9]                                外部异常类型    [1：异常发生于 缓存维护等相关指令 或者 执行地址转换指令]
 * CM[8]                                高速缓存维护
 * SIPTW[7]                             [1 异常错误来自从 stage 2 到 stage 1的页表转换]
 * WnR[6]                               [0 读] [1 写]
 * DFSC[5:0]                            数据异常状态码
 */
ESR_ELn

ISS 枚举

ISV[24]		[0 23:14 无效 1 23:14 有效]
SAS[23:22]	[访问宽度 00 byte 01 halfword 10 word 11 doubleword]
SSE[21]		[value写入的值]
SRT[20:16]	[value写入的值 对应寄存器index]
SF[15]		[目标寄存器宽度 0 32bit 1 64bit]
AR[14]
VNCR[13]
FnV[10]		[FAR有效 0 有效 1 无效]
EA[9]
CM[8]
S1PTW[7]	[1 stage-2转换异常]
WnR[6]		[0 R 1 W]
DFSC[5:0]

DFSC 枚举

0b000000	地址大小错误; L0页表或者页表地址基地址寄存器发生大小错误
0b000001	L1页表发生地址大小错误
0b000010	L2页表发生地址大小错误
0b000011	L3页表发生地址大小错误
0b000100	L0页表转换错误
0b000101	L1页表转换错误
0b000110	L2页表转换错误
0b000111	L3页表转换错误
0b001001	L1页表访问标志位错误
0b001010	L2页表访问标志位错误
0b001011	L3页表访问标志位错误
0b001101	L1页表访问权限错误
0b001110	L2页表访问权限错误
0b001111	L3页表访问权限错误
0b010000	外部访问错误 不是查询页表的错误
0b010001	标签检查错误
0b010100	查询页表时 在查询L0页表时发生错误
0b010101	查询页表时 在查询L1页表时发生错误
0b010110	查询页表时 在查询L2页表时发生错误
0b010111	查询页表时 在查询L3页表时发生错误
0b011000	访问内存时 在同步奇偶校验或者ECC时发生错误
0b011100	在同步奇偶校验或者ECC时发生错误 错误发生在查询L0页表时
0b011101	在同步奇偶校验或者ECC时发生错误 错误发生在查询L1页表时
0b011110	在同步奇偶校验或者ECC时发生错误 错误发生在查询L2页表时
0b011111	在同步奇偶校验或者ECC时发生错误 错误发生在查询L3页表时
0b100001	对齐错误
0b110000	TLB冲突


/*
 * Fault Address Register
 * stage-1阶段 保存同步异常的地址 这个地址只有OS能解析 对应 TTBRn_EL1
 */
FAR_ELn

/*
 * [Virtualization Translation Control 寄存器]
 * VS[19] [vmid bit位数 0 8bit 1 16bit]
 * PS[18:16] [stage-2页表控制]
 * TG0[15:14] [VTTBR_EL2的粒度 00 4k 01 64k 10 16k]
 */
VTCR_EL2

/*
 * Hypervisor IPA Fault Address Register
 * 经过stage-2阶段转换后 发生#PF的地址 保存#PF的IPA hyp保存了IPA的映射 例如 [设备虚拟化mmio rw异常上报]
 */
HPFAR_EL2


/*
 * Counter-timer Virtual Offset Register [Holds the 64-bit virtual offset]
 */
CNTVOFF_EL2


/*
 * Domain Access Control Register
 */
DACR32_EL2


/*
 * Instruction Fault Status Register
 */
IFSR32_EL


/*
 * Auxiliary Fault Status Register
 */
AFSR0_EL1
AFSR0_EL2
AFSR0_EL3
AFSR1_EL1
AFSR1_EL2
AFSR1_EL3

/*
 * Context ID Register [当前Process ID]
 */
CONTEXTIDR_EL1
CONTEXTIDR_EL2

/*
 * [Vector Base Address 寄存器] 异常向量表
 */
vbar_elx

/*
 * [缓存控制 寄存器]
 * https://blog.csdn.net/sinat_32960911/article/details/127856639
 */
ctr_elx

/*
 * Stage-1的地址转换 输出 IPA
 * ttbr0_el1 用户态使用 [0x000000000000     至 0x0000FFFFFFFF]
 * ttbr1_el1 内核态使用 [0xFFFF000000000000 至 0xFFFFFFFFFFFFFFFF]
 * Address Space ID[63:48]
 * Base ADDR[47:1]
 * CnP[0]
 */
TTBR0_EL1
TTBR0_EL2
TTBR0_EL3
TTBR1_EL1
TTBR1_EL2

/*
 * 将IPA转换成PA stage-2 转换使用
 * VMID[63:48]
 * BADDR[47:0]
 */
VTTBR_EL2
/*
 * Interrupt Controller List Registers
 * State[63:62]	[表示中断状态位；00:Invalid:01:Pending;10:Active;11:Pending and active]
 * HW[61]	[是否映射到硬件中断]
 * Group[60]	[0 group0 1 group1]
 * NMI[59]	[0 no-nmi 1 nmi]
 * Priority[55:48]	[优先级]
 * pINTID[44:32]	[物理中断号]
 * vINTID[31:0]		[虚拟中断号]
 */
ICH_LRn_EL2
/*
 * [系统控制 寄存器]
 * https://blog.csdn.net/sinat_32960911/article/details/127856639
 * EE[25] 异常字节序 [1 big]
 * EOE[24]
 * I[12] 指令cache enable
 * C[2] data cache enable
 * A[1] 对齐检查
 * M[0] MMU enable for stage-1
 */
SCTLR_EL2

/*
 * [安全配置 寄存器]
 * https://blog.csdn.net/sinat_32960911/article/details/127856639
 */
SCR_EL3

/*
 * [Hyp 控制 寄存器]
 * IMO[4] 所有irq将路由到EL2
 * VM[0] enable stage-2转换
 */
HCR_EL2

/*
 * [Count Timer Hyp控制 寄存器]
 */
cnthctl_el2

/*
 * [Count Timer Physical 寄存器]
 */
CNTHP_CVAL_EL2

/*
 * [Count Timer Physical 寄存器]
 */
CNTHP_CTL_EL2


/*
 * [AArch64 Memory Model Feature Register 提供基础内存信息]
 * TGran4_2[43:40]	4k粒度 for stage-2
 * TGran4[31:28]	4k粒度 for stage-1
 * BigEnd[11:8]		大小端
 * ASID[7:4]		0b0000: 8 bits.
 * PARange[3:0]		物理地址范围	[0b0100 44 bits, 16TB]
 */
id_aa64mmfr0_el1

/*
 * [虚拟化ProcessorID 寄存器] 和 midr_el1 一致
 */
vpidr_el2

/*
 * [Main ID 寄存器] PE 硬件厂商信息
 */
midr_el1

/*
 * [Architectural Feature Trap 寄存器]
 */
CPTR_EL2

msr             /* 特殊寄存器读写指令 */
msr SPSel, #0   // switch to SP_EL0
msr SPSel, #1   // switch to SP_EL1

/*
 * 参数传递和返回值存放
 * X0 存放 返回值
 */
X0 ～ X7
/*
 * X8 别名 XR
 * 当返回值为基本数据类型时 存放在 X0中
 * 如果返回值为 结构体对象 使用X8寄存器返回
 */
X8
/*
 * 直接使用即可 无需  [save - use - restore]
 */
 X9 ～ X15
 /*
  *
  */
 X16   X17
 /*
  * platform register 平台寄存器
  */
 X18
 /*
  * callee saved registers
  */
x19 ~ X28
/*
 * FP 寄存器 frame pointer
 */
X29
/*
 * ret 返回地址
 * LR 寄存器
 */
X30
/*
 * 用于清零的寄存器
 */
XZR
/*
 *
 */
PC


// TODO - [异常处理流程]

1 PSTATE 保存到目标异常级别的SPSR_ELx中;

2 返回地址 保存到目标异常级别的ELR_ELx中;

3  PSTATE 寄存器里面的 DAIF 域都设置为1 关闭中断

4 如果异常是同步异常或SError中断 异常的表征信息将保存在目标异常级别的ESR_ELx中;

4 如果是指令止异常(Instruction Abort) 数据中止异常(Data Abort) PC对齐错误异常(PC alignment fault),故障的虚拟地址将保存在FAR_ELx中;

5 堆栈指针保存到目标异常级别的专用堆栈指针寄存器SP_ELx;

6 执行移至目标异常级别 并从异常向量定义的地址开始执行;


// TODO - [异常处理返回]

1 从 ELR_ELx 寄存器中恢复 PC 指针

2 恢复PSTATE

// TODO - [linux kernel 异常向量表]


/*
 * el 入参
 * ht 入参
 * regsize 入参
 * label 入参
 */
.macro kernel_ventry, el:req, ht:req, regsize:req, label:req
	.align 7
	sub	sp, sp, #PT_REGS_SIZE
	b	el\el\ht\()_\regsize\()_\label


SYM_CODE_START(vectors)
	kernel_ventry	1, t, 64, sync		// Synchronous EL1t		el1t_64_sync_handler
	kernel_ventry	1, t, 64, irq		// IRQ EL1t
	kernel_ventry	1, t, 64, fiq		// FIQ EL1t
	kernel_ventry	1, t, 64, error		// Error EL1t

	kernel_ventry	1, h, 64, sync		// Synchronous EL1h		el1h_64_sync_handler
	kernel_ventry	1, h, 64, irq		// IRQ EL1h
	kernel_ventry	1, h, 64, fiq		// FIQ EL1h
	kernel_ventry	1, h, 64, error		// Error EL1h

	kernel_ventry	0, t, 64, sync		// Synchronous 64-bit EL0	el0t_64_sync_handler
	kernel_ventry	0, t, 64, irq		// IRQ 64-bit EL0
	kernel_ventry	0, t, 64, fiq		// FIQ 64-bit EL0
	kernel_ventry	0, t, 64, error		// Error 64-bit EL0

	kernel_ventry	0, t, 32, sync		// Synchronous 32-bit EL0	el0t_32_sync_handler
	kernel_ventry	0, t, 32, irq		// IRQ 32-bit EL0
	kernel_ventry	0, t, 32, fiq		// FIQ 32-bit EL0
	kernel_ventry	0, t, 32, error		// Error 32-bit EL0
SYM_CODE_END(vectors)


// TODO - [AArch64 AAPCS]

[Arch ARM Procedure Call Standard]

x31	SP	堆栈指针寄存器，函数调用栈栈顶，低地址
x30	LR	链接寄存器，函数返回地址
x29	FP	栈帧寄存器，函数调用栈栈底，高地址
x19…x28		Callee-saved registers
x18		The Platform Register, if needed; otherwise a temporary register.
x17	IP1	\
x16	IP0	\
x9…x15		Caller-saved temporary registers
x8		Indirect result location register
x0…x7		Parameter/result registers


