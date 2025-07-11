

//TODO - linux wireshark


Ip.addr == 10.43.11.11&& udp && !mdns && !nbns && !browser &&!ntp



//TODO - linux ps


ps aux --sort -rss | head  /* 内存消耗最大 */
ps -eo pid,ppid,%mem,%cpu,cmd --sort=-%mem | head  /* 仅展示有关内存消耗过程的特定信息 */


//TODO - linux mm meminfo

$cat /proc/meminfo

MemTotal:        8052444 kB   /* OS 可见内存 */
MemFree:         2754588 kB   /* 未使用内存 lowFree + highFree */
MemAvailable:    3934252 kB   /* 在不使用交换空间的情况下 启动一个新的应用最大可用内存的size */
Buffers:          137128 kB #块设备所占用的缓存页 包括：直接读写块设备以及文件系统元数据(metadata)
	比如superblock使用的缓存页。
Cached:          1948128 kB   /* 表示普通文件数据所占用的pagecache */
SwapCached:            0 kB #swap cache中包含的是被确定要swapping换页 但是尚未写入物理交换区的\
	匿名内存页。那些匿名内存页 比如用户进程malloc申请的内存页是没有关联任何文件的
	如果发生swapping换页 这类内存会被写入到交换区。
Active:          3650920 kB #active包含active anon和active file
Inactive:        1343420 kB #inactive包含inactive anon和inactive file
Active(anon):    2913304 kB #anonymous pages 匿名页 用户进程的内存页分为两种：\
	与文件关联的内存页(比如程序文件,数据文件对应的内存页)和与内存无关的内存页 比如进程的堆栈 \
	用malloc申请的内存 前者称为file pages或mapped pages,后者称为匿名页。
Inactive(anon):   727808 kB #见上
Active(file):     737616 kB #见上
Inactive(file):   615612 kB #见上
SwapTotal:       8265724 kB #可用的swap空间的总的大小
SwapFree:        8265724 kB #当前剩余的swap的大小
Dirty:               104 kB #需要写入磁盘的内存去的大小
Writeback:             0 kB #正在被写回的内存区的大小
AnonPages:       2909332 kB #未映射页的内存的大小
Mapped:           815524 kB #设备和文件等映射的大小
Shmem:            732032 kB #共享内存大小
Slab:             153096 kB #内核数据结构slab的大小
SReclaimable:      99684 kB #可回收的slab的大小
SUnreclaim:        53412 kB #不可回收的slab的大小
KernelStack:       14288 kB
PageTables:        62192 kB
NFS_Unstable:          0 kB
Bounce:                0 kB
WritebackTmp:          0 kB
CommitLimit:    12291944 kB
Committed_AS:   11398920 kB
VmallocTotal:   34359738367 kB
VmallocUsed:           0 kB
VmallocChunk:          0 kB
HardwareCorrupted:     0 kB
AnonHugePages:   1380352 kB
CmaTotal:              0 kB
CmaFree:               0 kB
HugePages_Total:       0
HugePages_Free:        0
HugePages_Rsvd:        0
HugePages_Surp:        0
Hugepagesize:       2048 kB
DirectMap4k:      201472 kB
DirectMap2M:     5967872 kB
DirectMap1G:     3145728 kB

WMARK_MIN 内存非常紧张 kswap回收
WMARK_LOW 内存紧张 需要唤醒kswap回收 直到WMARK_HIGH




//TODO - linux adb

adb connect 10.43.119.136
adb connect


//TODO - linux slab

slabtop


