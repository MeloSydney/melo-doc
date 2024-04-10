

//TODO - linux vdbench


echo -e data_errors=1 sd=default,journal=/root/ps3test/vdbench/journal,\
openflags=directio.threads=8

sd=sd1,lun=/dev/sde

wd=default,xfersize=(32k,25,64k,25,512k,25,512k,25)






//TODO - linux fio

fio -filename=/dev/emcpowerb -direct=1 -iodepth 1 -thread -rw=randread \
-ioengine=psync -bs=4k -size=1000G -numjobs=50 -runtime=180 -

fio -name=mode4 -direct=1 -thread=1 -time_based=1 -ioengine=libaio -numjobs=1 \
-iodepth=100 -runtime=60 -rw=write -bs=512 -filename=/dev/sdb:/dev/sdh

fio -name=mode4 -direct=1 -thread=1 -time_based=1 -ioengine=libaio -numjobs=1 \
-iodepth=32 -runtime=60 -rw=randrw -percentage_random=63 -bs=1310208 \
-filename=/dev/sdb:/dev/sdh

fio -filename=/dev/sda -direct=1 -iodepth=100 -thread=1 -rw=randrw -rwmixread=30 \
-ioengine=libaio -bs=31k -bsrange=512 -20480 -size=10G -numjobs=1 -runtime=180 \
-group_reporting -name=randrw_

fio -filename=/dev/sda:/dev/sdd:/dev/sde:/dev/sdf:/dev/sdh:/dev/sdc-direct=1 -iodepth=100 \
-thread=1 -rw=randrw -rwmixread=30 -ioengine=libaio -bs=1281k -bsrange=512 -20480 \
-size=10G -numjobs=1 -runtime=180 -group_reporting -name=randrw_

fio -name=melo -direct=1 -thread=1 -time_based=1 -ioengine=libaio -numjobs=1 \
-iodepth=100 -runtime=60 -rw=randrw-bs=4k -filename=/dev/sdb

fio -filename=/dev/sdc -direct=1 -iodepth 128 -thread -rw=write -ioengine=libaio \
-bs=4k -size=10G -numjobs=1 -runtime=120 -group_reporting -name=mytest --time_based

fio -name=melo -direct=1 -thread=1 -time_based=1 -ioengine=libaio -numjobs=1 \
-iodepth=32 -runtime=60 -rw=randrw -bs=4k -filename=/dev/sdm // 跑过的

fio -filename=/dev/sda -direct=1 -iodepth=128 -thread -rw=randread \
-ioengine=libaio -bs=4k -size=200G -numjobs=1 -runtime=120

//TODO - linux fio 结果

aggrb // 最后总宽带 可以换算成iops

slat: min 最小 max最大 avg平均 stdev标准差  clat命令执行时间  lat总延迟

90.00th=[ 684] // 90%的读命令在684内回复


//TODO - linux fio config

[global]
verify=md5
do_verify=1
thread
direct=1      /* 不用io buffer */
time_based    /* 如果在 runtime 指定的时间还没到时文件就被读写完成，将继续重复知道 runtime 时间结束 */
group_reporting
numjobs=1
verify_dump=1
rw=randrw     /* 随机读写 */
rwmixread=0   /* 混合读写模式下 read占0% */
percentage_random=0
ioengine=libaio
filename=/dev/sdb
runtime=120
iodepth=4
serialize_overlap=1
