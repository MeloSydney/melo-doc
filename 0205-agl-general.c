


//TODO - agl qemu 拉取代码

ln -f -s ./repository/repo.git/repo ./repo  /* 配置 repo */
#! /bin/bash

username="$(git [configuser.name](http://configuser.name))"
git clonessh://${username}@xcserver:29418/repo_new ./repository/repo.git



./repo init -u \
ssh://${username}@xcserver:29418/projects/AMD_PLATFORM/AGL/manifests \
-b evb_g4_normal_dev \
-m default.xml

./repo sync -j16

./repo start --all evb_g4_normal_dev

./mini_agl_code_v12.sh


//TODO - agl qemu 编译

cd /data1/hexudong/git/agl/agl

./build.py amd

//TODO - agl qemu 验证

sudo apt install qemu-system-x86

将构建的映像文件拷贝至Ubuntu机器中。包括：bzImage和ecarx-image-weston-qemux86-64.ext4

路径: agl/build/tmp/deploy/images/qemux86-64

#!/bin/sh

qemu-system-x86_64 \
  -name AGL12 \
  -machine q35 \
  -cpu kvm64 \
  -cpu qemu64,+ssse3,+sse4.1,+sse4.2,+popcnt\
  -enable-kvm \
  -m 2048 \
  -device intel-hda -device hda-duplex \
  -display gtk,gl=on \
  -vga virtio \
  -devicevirtio-net-pci,netdev=net0,mac=52:54:00:12:35:02 \
  -netdevuser,id=net0,hostfwd=tcp::2222-:22,hostfwd=tcp::3333-:3333 \
  -drivefile=ecarx-image-weston-qemux86-64.ext4,if=virtio,format=raw \
  -kernel bzImage \
  -usb \
  -device usb-tablet \
  -device virtio-rng-pci \
  -snapshot \
  -serial mon:vc \
  -serial mon:stdio \
  -serial null \
  -device vhost-vsock-pci,id=vhost-vsock-pci0,guest-cid=3\
  -objectmemory-backend-file,size=16M,mem-path=/dev/shm/sharefb,share=on,id=sharefb \
  -deviceivshmem-plain,memdev=sharefb,master=on,addr=0x06 \
  -append 'root=/dev/vda rw console=tty0mem=2048M ip=dhcp oprofile.timer=1 \
  console=ttyS0,115200n8 verbose fstab=nono-kvmclock quiet' &

sudo ./start-agl-qemu4.sh

/meta-ecarx-bsp/meta-amd/conf/machine/amd-evb.conf:PREFERRED_VERSION_weston= "8.0.0"

