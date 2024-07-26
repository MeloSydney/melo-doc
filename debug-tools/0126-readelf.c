


//TODO - linux readelf

READELF
查看共享库或者二进制文件是否包含符号，
可以检查gdb是否能设置断点
readelf -s {binary/so}


readelf -h hello                        /* 查看 elf header */

readelf -S hello                        /* 查看每个段 */

readelf -Ws --dyn-syms amdgpu_drv.so    /* 查看符号表 */

readelf -x .shstrtab   vmlinux          /* 读取hex  section .shstrtab */

readelf -l vmlinux                      /* 读取 program header table */
//TODO - linux nm

/* 查看hello内部符号 */
nm hello