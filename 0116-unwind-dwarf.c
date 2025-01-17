
// TODO - [dward refs]


[手动推导 dwarf]
https://zhuanlan.zhihu.com/p/17821766342



// TODO - [dwarf CFI]

[DWARF Debugging With Attributed Record Formats]

[CFI Call Frame Information .eh_frame 这个section应该有一个CFI]

[CIE Common Information Entry 一个CFI对应一个CIE]

typedef struct dwarf_cie_info {
	unw_word_t cie_instr_start; /* start addr. of CIE "initial_instructions" */
	unw_word_t cie_instr_end;   /* end addr. of CIE "initial_instructions" */
	unw_word_t fde_instr_start; /* start addr. of FDE "instructions" */
	unw_word_t fde_instr_end;   /* end addr. of FDE "instructions" */
	unw_word_t code_align;      /* code-alignment factor */
	unw_word_t data_align;      /* data-alignment factor */
	unw_word_t ret_addr_column; /* column of return-address register */
	unw_word_t handler;         /* address of personality-routine */
	uint16_t abi;
	uint16_t tag;
	uint8_t fde_encoding;
	uint8_t lsda_encoding;
	unsigned int sized_augmentation : 1;
	unsigned int have_abi_marker : 1;
	unsigned int signal_frame : 1;
} dwarf_cie_info_t;


[FDE Frame Description Entry 对应一个函数 一个CIE对应多个FDE]

[CFA Canonical Frame Address (A->B) 是A的栈顶 是B的栈底]
[例如 CFA可以通过当前sp指针获取 其他通用寄存器可以通过CFA获取]
[c-80 == CFA - 80]

00000934 000000000000006c 00000938 FDE cie=00000000 pc=0000000000009630..0000000000009834
   LOC           CFA      x19   x20   x21   x22   x23   x24   x25   x26   x27   x28   x29   ra
0000000000009630 sp+0     u     u     u     u     u     u     u     u     u     u     u     u
0000000000009634 sp+96    u     u     u     u     u     u     u     u     u     c-96  c-88
000000000000963c sp+96    c-80  c-72  u     u     u     u     u     u     u     u     c-96  c-88
0000000000009648 sp+96    c-80  c-72  u     u     u     u     c-32  c-24  u     u     c-96  c-88
0000000000009660 sp+96    c-80  c-72  c-64  c-56  u     u     c-32  c-24  u     u     c-96  c-88
0000000000009668 sp+96    c-80  c-72  c-64  c-56  c-48  c-40  c-32  c-24  u     u     c-96  c-88
0000000000009670 sp+96    c-80  c-72  c-64  c-56  c-48  c-40  c-32  c-24  c-16  c-8   c-96  c-88
0000000000009758 sp+96    c-80  c-72  u     u     c-48  c-40  c-32  c-24  c-16  c-8   c-96  c-88
0000000000009760 sp+96    c-80  c-72  u     u     u     u     c-32  c-24  c-16  c-8   c-96  c-88
0000000000009768 sp+96    c-80  c-72  u     u     u     u     c-32  c-24  u     u     c-96  c-88
000000000000978c sp+0     u     u     u     u     u     u     u     u     u     u     u     u
0000000000009790 sp+96    c-80  c-72  c-64  c-56  c-48  c-40  c-32  c-24  c-16  c-8   c-96  c-88
0000000000009808 sp+96    c-80  c-72  u     u     u     u     c-32  c-24  u     u     c-96  c-88
0000000000009830 sp+0     u     u     u     u     u     u     u     u     u     u     u     u

// TODO - [dwarf readelf]

[readelf -wF /usr/bin/ls 获取.debug_frame]

[readelf -wf /usr/bin/ls 获取.eh_frame]



// TODO - [dwarf pc重映射]

[0x557bacdb145d 运行时 pc地址]
[0x557bac5d7000 运行时 映射基地址]
[0x35a000       mmap  代码段映射偏移]
(gdb) p /x (0x557bacdb145d - 0x557bac5d7000 + 0x35a000)
$6 = 0xb3445d

