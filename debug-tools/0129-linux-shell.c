

//TODO - shell 参数

!^  参数1
!$  参数last
!:N  参数N




//TODO - shell 快捷键 shorcut

ctrl u  向前剪切
ctrl k  向后剪切
ctrl y  粘贴
ctrl e  行首
ctrl a  行尾
alt f   后跳
alt b 前跳
alt backspace  前删除
alt w  前剪切一个word
shift insert  复制
ctrl z  前台切换后台 background
jobs -l  查看后台pid
fg %2  切换后台进程到前台
bg "%wget"  切换到后台
fg  唤醒后台
nohop  下载或传输大文件使用




//TODO - shell 硬复位测试

```text
#!/bin/bash

i=0

while [ $i -ne 10 ]
do
	./cli_x86 /c0 show all
	./cli_x86 /c0 add vd r1 drives=0:1-2
	./cli_x86 /c0 add vd r0 drives=0:3-4
	./cli_x86 /c0 /vall del
	lsscsi

	sg_inq /dev/sdf
# fio -time_based=1 -rw=randrw -ioengine=libaio -bssplit=4k
-numjobs=32 -iodepth=1 -runtime=20 -rwmixread=50 -percentage_random=100
-do_verify=1 -verify=md5 -name=1 -filename=/dev/sdf
	sg_inq /dev/sdc
# fio -time_based=1 -rw=randrw -ioengine=libaio -bssplit=4k
-numjobs=32 -iodepth=1 -runtime=20 -rwmixread=50 -percentage_random=100
-do_verify=1 -verify=md5 -name=1 -filename=/dev/sdc
	let i++
done
```

```text
#!/bin/sh
devceinfo=$(lspci|grep 0002)

devpath="/sys/bus/pci/devices/0000:"
bdf=${devceinfo:0:7}
bar2="/resource2"
path=$devpath$bdf$bar2
i=1
fw_state=0
for((i=0; i<100000; i++))
do
  #setpci -s 32:0.0 0x18.l=0xb0400004
  #setpci -s 32:0.0 0x4.b=0x6
  lspci -s $bdf -vvvx|grep LnkS
  echo "-----input hardreset key within 1s"
  ./pcimem $path 0x200 d 0x52
  ./pcimem $path 0x200 d 0x5F
  ./pcimem $path 0x200 d 0x55
  ./pcimem $path 0x200 d 0x5F
  ./pcimem $path 0x200 d 0x52
  ./pcimem $path 0x200 d 0x45
  ./pcimem $path 0x200 d 0x41
  ./pcimem $path 0x200 d 0x44
  ./pcimem $path 0x200 d 0x59
  hardinfo=$(./pcimem $path 0x208 d)
  hardstate=${hardinfo:36:37}
  echo "hardstate: "$hardstate
  if [[ $hardstate == "0001" ]]; then
    echo "check hardreset state right key"
    echo "-----send a hardreset signal"
    ./pcimem $path 0x210 d 0x1
  else
    echo "check hardreset state error key"
  fi

  sleep 10s

  fw_state=$(./pcimem $path 0x300 d)
  fw_value=${fw_state:35:37}
  while [[ $fw_value != "00902" ]]
  do
    fw_state=$(./pcimem $path 0x300 d)
    fw_value=${fw_state:35:37}
    sleep 1s
    echo "fw_state: "$fw_value
  done
  sleep 30s
  echo "#######################loop_cnt=$i###############################"
done
```





//TODO - shell 硬复位测试

```text
#!/bin/bash

i=0

while [ $i -ne 10 ]
do
  ./cli_x86 /c0 show all
  ./cli_x86 /c0 add vd r1 drives=0:1-2
  ./cli_x86 /c0 add vd r0 drives=0:3-4
  ./cli_x86 /c0 /vall del
  lsscsi

  sg_inq /dev/sdf
#  fio -time_based=1 -rw=randrw -ioengine=libaio -bssplit=4k
-numjobs=32 -iodepth=1 -runtime=20 -rwmixread=50 -percentage_random=100
-do_verify=1 -verify=md5 -name=1 -filename=/dev/sdf
  sg_inq /dev/sdc
#  fio -time_based=1 -rw=randrw -ioengine=libaio -bssplit=4k
-numjobs=32 -iodepth=1 -runtime=20 -rwmixread=50 -percentage_random=100
-do_verify=1 -verify=md5 -name=1 -filename=/dev/sdc
  let i++
done
```

```text
fw_state=0
devpath="/sys/bus/pci/devices/0000:"
bdf=${devceinfo:0:7}
bar2="/resource2"
path=$devpath$bdf$bar2
rn=0
while [ true ]
do
  ./cli_x86 driver hard_reset host_no 14

  fw_state=$(./pcimem $path 0x300 d)
  fw_value=${fw_state:35:37}
  echo "fw_state: "$fw_value "run num:$rn"
  rn=`expr $rn + 1`
  sleep 90s
  i=0
  while [[ $fw_value == "00000" ]]
  do
    fw_state=$(./pcimem $path 0x300 d)
    fw_value=${fw_state:35:37}
    date -R
    echo "fw_state: "$fw_value "index:$i"
    i=`expr $i + 1`
    sleep 5s
  done
done
```