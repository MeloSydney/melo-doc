


//TODO - acrn menuentry

```text
#!/bin/sh
exec tail -n +3 $0

menuentry 'ACRN multiboot2' --id ACRN_deb_multiboot2 {
	recordfail
	load_video
	gfxmode $linux_gfx_mode
	insmod gzio
	if [ x$grub_platform = xxen ]; then insmod xzio; insmod lzopio; fi
	insmod part_gpt
	insmod btrfs
	set root='hd1,gpt2'
	if [ x$feature_platform_search_hint = xy ]; then
		search --no-floppy --fs-uuid --set=root --hint-bios=hd1,gpt2 \
		--hint-efi=hd1,gpt2 --hint-baremetal=ahci1,gpt2  \
		32709404-cd04-4e38-b556-f3d1a6d0a788
	else
		search --no-floppy --fs-uuid --set=root \
		32709404-cd04-4e38-b556-f3d1a6d0a788
	fi
	# 上面抄的grub中其他entry

	echo    'Loading acrn hypervisor ...'
	# Has a active serial
	multiboot2 /@/boot/acrn.hp280_g3_board.shared.bin
		root=PARTUUID="9c4cdc8e-6d06-4471-b975-bc8aa251d2ac"
		uart=port@0x3f8,115200
	# port地址需要和bios统一

	/* # Don't have a active serial */
	# 重新安装文件系统sda2 btrfs 可以使用/@
	# sda1 efi
	# sdb1 ext4
	# Ubuntu kernel
	# echo    'Loading Linux vmlinuz-5.13.0-51-generic  ...'
	# module2 /@/boot/vmlinuz-5.13.0-51-generic Linux_bzImage \
	root=UUID="32f325a7-131f-45c2-ab6a-e896546271f8" ro rootflags=subvol=@ \
	console=tty0 console=ttyS0,115200n8 quiet splash idle=nomwait iomem=relaxed \
	intel_idle.max_cstate=0 intel_pstate=disable $vt_handoff
	# echo  'Loading initial ramdisk ...'
	# module2  --nounzip /@/boot/initrd.img-5.13.0-51-generic
	echo    'Loading  acrn-service-vm ...'
	module2 /@/boot/vmlinuz-5.10.115-acrn-service-vm Linux_bzImage \
		root=UUID="32f325a7-131f-45c2-ab6a-e896546271f8" ro rootflags=subvol=@ \
		console=tty0 console=ttyS0,115200n8
	echo    'Loading initial ramdisk ...'
	module2 --nounzip   /@/boot/initrd.img-5.10.115-acrn-service-vm
}


//TODO - acrn startup.sh


if [ x$1 == xdebug ]; then
echo "debugging..."
#sudo  gdb -x qemu-dbg.txt --args  /data1/zhanghuaqiang/qemu-5.1.0-install/bin/qemu-system-x86_64 \
	-name guest=ubuntu-20.04.4,debug-threads=on -machine pc-i440fx-3.1,accel=kvm \
	-cpu Broadwell-noTSX-IBRS -m 8192 -smp 4,sockets=4,cores=1,threads=1 \
	-uuid 184c52cc-8625-4c15-96fc-bc8015755f5c -no-user-config -nodefaults -global PIIX4_PM.disable_s3=1 \
	-global PIIX4_PM.disable_s4=1 -device ich9-usb-ehci1,id=usb,bus=pci.0,addr=0x5.0x7 \
	-device ich9-usb-uhci1,masterbus=usb.0,firstport=0,bus=pci.0,multifunction=on,addr=0x5 \
	-device ich9-usb-uhci2,masterbus=usb.0,firstport=2,bus=pci.0,addr=0x5.0x1 \
	-device ich9-usb-uhci3,masterbus=usb.0,firstport=4,bus=pci.0,addr=0x5.0x2 \
	-drive file=/home/ffjerry/work/qemuimg/ubuntu20.04-acrn.raw,format=raw,if=none,id=drive-ide0-0-0 \
	-device ide-hd,bus=ide.0,unit=0,drive=drive-ide0-0-0,id=ide0-0-0,bootindex=1 \
	-drive if=none,id=drive-ide0-0-1,readonly=on -device virtio-tablet-pci,id=input2,bus=pci.0,addr=0x8 \
	-device virtio-keyboard-pci,id=input3,bus=pci.0,addr=0x9 \
	-net nic,macaddr='32:36:a0:16:34:38',model=virtio -device virtio-gpu-pci -display sdl,gl=on \
	-net tap
sudo  /data1/zhanghuaqiang/qemu-5.1.0-install/bin/qemu-system-x86_64 \
	-**kernel **/home/ffjerry/work/acrn-ins/virt-kernel/bzImage \
	-**initrd **/home/ffjerry/work/acrn-ins/virt-kernel/initrd.img-5.15.35+  \
	-append 'root=/dev/sda2 console=tt0 console=ttyS0,115200 loglevel=7 drm.debug=0x1ff nokaslr' \
	-name guest=ubuntu-20.04.4,debug-threads=on \
	-machine pc-i440fx-3.1,**accel=kvm** -cpu Broadwell-noTSX-IBRS \
	-**m **8192 -smp 4,sockets=4,cores=1,threads=1 -uuid 184c52cc-8625-4c15-96fc-bc8015755f5c \
	-no-user-config -nodefaults -global PIIX4_PM.disable_s3=1 -global PIIX4_PM.disable_s4=1 \
	-device ich9-usb-ehci1,id=usb,bus=pci.0,addr=0x5.0x7 \
	-device ich9-usb-uhci1,masterbus=usb.0,firstport=0,bus=pci.0,multifunction=on,addr=0x5 \
	-device ich9-usb-uhci2,masterbus=usb.0,firstport=2,bus=pci.0,addr=0x5.0x1 \
	-device ich9-usb-uhci3,masterbus=usb.0,firstport=4,bus=pci.0,addr=0x5.0x2 \
	-**drive **file=/home/ffjerry/work/qemuimg/ubuntu20.04-acrn.raw,format=raw \
	-device virtio-tablet-pci,id=input2,bus=pci.0,addr=0x8 \
	-device virtio-keyboard-pci,id=input3,bus=pci.0,addr=0x9 \
	-net nic,macaddr='32:36:a0:16:34:38',model=virtio \
	-device virtio-gpu-pci -display sdl,gl=on \
	-net tap,script=./qemu-ifup,downscript=./qemu-ifdown -serial stdio -S -s
else
	echo "direct run...."
	...
fi



//TODO - acrn ifup ifdown

#! /bin/sh
# Script to shut down a network (tap) device for qemu.
# Initially this script is empty, but you can configure,
# for example, accounting info here.

#! /bin/sh
switch=virbr0
brctl delif ${switch} $1
ifconfig $1 down
#ip link set $1 down
#tunctl -d $1

**qemu-ifdown**

#! /bin/sh
switch=virbr0
ifconfig $1 up
 #ip link set $1 up
brctl addif ${switch} $1

**qemu-ifup**



//TODO - acrn oom when startup

/* https://unix.stackexchange.com/questions/270390/how-to-reduce-the-size-of-the-initrd-when-compiling-your-kernel */
SHW@SHW:/tmp# cd /lib/modules/<new_kernel>
SHW@SHW:/tmp# find . -name *.ko -exec strip -v --strip-unneeded {} +






