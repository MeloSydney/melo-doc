
//TODO - armv8 register

/*
 * 当前runtime状态寄存器
 */
CPSR_ELx
/*
 * 保存 CPSR
 * SPSR_EL1, SPSR_EL2, SPSR_EL3
 * M[4] 运行模式 aarch64=0X0 aarch32=0X1
 * M[3:0] 异常等级
 */
SPSR

/*
 * eret 返回地址
 * ELR_EL1, ELR_EL2, ELR_EL3
 */
ELR
/*
 * stack pointer
 * SP_EL0, SP_EL1, SP_EL2, SP_EL3
 */
SP_ELx
/*
 * 保存发生异常时的特征，比如异常分类 ESR_ELx.EC 异常具体原因 ESR_ELx.ISS 访问的字节数 目标寄存器 load还是store
 */
ESR_ELn
/*
 * 保存错误地址
 */
FAR_ELn
/*
 * stage-2阶段 地址转换发生的错误IPA地址
 */
HPFAR_EL2
/*
 * 完成Stage 1的地址转换 输出 IPA
 */
TTBRn_EL1
/*
 * 将IPA转换成PA
 */
VTTBR_EL2


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



