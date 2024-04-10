


//TODO - linux readelf

READELF
查看共享库或者二进制文件是否包含符号，
可以检查gdb是否能设置断点
readelf -s {binary/so}


/* 查看header */
readelf -h hello

/* 查看每个段 */
readelf -S hello


//TODO - linux nm

/* 查看hello内部符号 */
nm hello