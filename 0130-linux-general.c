


// TODO - linux 前后台 任务 切换 fg bg jobs

jobs -l                                 /* 查看后台任务 */
fg %{d}                                 /* 切换后台{d} 到前台 */
ctrl-z                                  /* 暂停前台任务 */
bg %{d}                                 /* 将后台{d} 切换到前台 */
strace -f -p 133 -o trace.txt &         /* 程序后台运行 */

// TODO - linux 查看 bin 依赖 so ldd

ldd ./modetest



// TODO - linux ubuntu sudo sudoer

sudo vi /etc/sudoers
%sudo  ALL=(ALL:ALL) **NOPASSWD:**ALL  // 修改


// TODO - linux ssh-copy-id 免密 windows

ssh-copy-id -i ~/.ssh/id_rsa.pub shifan@10.43.48.240
/* windows下 将windows公钥拷贝到对端Linux服务器 */
type C:\Users\melo\.ssh\id_rsa.pub | ssh sf443845@30.178.38.61 "cat >> .ssh/authorized_keys"

A需要免密登录B 就要将A的公钥告知B


// TODO - linux diff

diff -uNr file1 file2


//TODO - linux reg 正则表达式

\s* 表示若干个空格（可以是0个）
\s+ 表示一个或多个空格
master_xfer\s+=  匹配多个空格


//TODO - cherry 键盘 锁定 win

https://blog.csdn.net/weixin_43487782/article/details/108739900


// TODO linux tmux config

vi ~/.tmux.conf

# Set Ctrl-b key bindings
unbind C-b
set -g prefix `

# Count sessions start at 1
set -g base-index 1

# Use vim bindings
setw -g mode-keys vi
# start selecting text typing 'v' key (once you are in copy mode)
bind-key -T copy-mode-vi 'v' send -X begin-selection
# # copy selected text to the system clipboard
bind-key -T copy-mode-vi 'y' send -X copy-pipe-and-cancel pbcopy

# split window
#unbind '"'
#bind - splitw -v
#unbind %
#bind \ splitw -h

# Status bar settings
# alignment
set-option -g status-justify centre

# left-bottom corner
set-option -g status-left '#[bg=black, fg=green][#[fg=cyan]#S#[fg=green]]'
set-option -g status-left-length 20

# window list
set-window-option -g window-status-format '#[dim]#I:#[default]#W#[fg=grey, dim]'
set-window-option -g window-status-current-format '#[fg=cyan, bold]#I#[fg=blue]:#[fg=cyan]#W#[fg=dim]'

# right-bottom status
set -g status-right '#[fg=green][#[fg=cyan]%Y-%m-%d#[fg=green]]'

# center window status
set -g window-status-format "#I:#W"
set -g window-status-current-format "#I:#W"

# default statusbar colors
set -g status-fg white
set -g status-bg black

# default window title colors
set-window-option -g window-status-style fg=white
set-window-option -g window-status-style bg=default
set-window-option -g window-status-style dim

# active window title colors
set-window-option -ag window-status-current-style fg=red
set-window-option -ag window-status-current-style bg=default
set-window-option -ag window-status-current-style bold

# window-name
set-option -g allow-rename off


https://xpgeng.gitbooks.io/omooc2py/content/guide/Tmux-Guide.html](https://xpgeng.gitbooks.io/omooc2py/content/guide/Tmux-Guide.html)
https://kxcblog.com/post/terminal/2.tmux-tutorial/](https://kxcblog.com/post/terminal/2.tmux-tutorial/)


tmux new -s "acrn-kernel-bld"
tmux attach -t acrn-bld
tmux ls


//TODO - linux vim shortcuts

:s/XXX/xxx/g        /* 全局替换 XXX >> xxx */

//TODO - linux 获取 栈帧 大小

/* file name: stacksize.c */
void *orig_stack_pointer;
void blow_stack()
{
    blow_stack();
}

int main() {
    __asm__("movl %esp,  orig_stack_pointer");
    blow_stack();
    return 0;
}

$ g++ -g stacksize.c -o ./stacksize
$ gdb ./stacksize
(gdb) r
Starting program: /home/home/misc-code/setrlimit

Program received signal SIGSEGV,  Segmentation fault.
blow_stack () at setrlimit.c:4
4       blow_stack();
(gdb) print (void *)$esp
$1 = (void *) 0xffffffffff7ff000
(gdb) print (void *)orig_stack_pointer
$2 = (void *) 0xffffc800
(gdb) print 0xffffc800-0xff7ff000
$3 = 8378368    // Current Process Stack Size is 8M


//TODO - linux 用户态 堆栈 打印

#define SIZE 100
void show_bt(void)
{
    int nptrs;
    void *buffer[100];
    nptrs = backtrace(buffer,  SIZE);
    printf("backtrace() returned %d addresses\n",  nptrs);
    backtrace_symbols_fd(buffer,  nptrs,  STDOUT_FILENO);
}


//TODO - linux virbr bridge

 sudo ifconfig virbr0 down
 sudo brctl delbr virbr0


//TODO - linux 解压 tar xz

xz   -d    libvirt-3.2.0.tar.xz
tar  xvf   libvirt-3.2.0.tar


//TODO - linux setpci

sudo setpci -s 00:02.1 F4.B=x
    For the PCI device with the ID 00:02.1,  set the register F4 (byte only) to x.


//TODO - linux minicom

minicom -b 115200 -D /dev/tty*  /* 波特率 设备 */

//TODO - linux sed

sed -n '5, 10p' filename        /* 查看file从开头数的5~10行 */
sed -n -e '/^101/ p' -e '/^103/ p' file.txt  /* 打印101 和103开头的 */
sed -n '2 p' file.txt          /* 只打印第二行 */
sed -n '1, 4 p' file.txt
sed -n '1, $ p' file.txt
sed -n '/Jane/ p' file.txt     /* 打印包含Jane的行 */
sed -n '/Raj/, /Jane/ p' file.txt  /* 打印包含Raj到Jane的行 */

sed '1, $ d' file.txt           /*  删除1, $行 */

sed 's/Manager/Director/' file.txt  /* 替换 */
sed '/Sales/s/Manager/Director/' file.txt   /* 替换包含Sales的行 一行 */
sed '/Sales/s/Manager/Director/g' file.txt  /* 替换包含Sales的行 所有 */
sed '/Sales/s/Manager/Director/2' file.txt  /* 替换包含Sales的行 第二个 */


//TODO - linux netstat

netstat -tlnp   /* 查看端口 */

//TODO - linux ioctl cmd

#define _IO(type, nr) //制作没有数据传输的命令
#define _IOR(type, nr, size) //制作读取数据的命令
#define _IOW(type, nr, size) //制作写入数据的命令
#define _IOWR(type, nr, size) //制作先写入后读取的双向传输命令

例如:
#define KVMFR_DMABUF_GETSIZE _IO('u',  0x44)
#define KVMFR_DMABUF_CREATE  _IOW('u',  0x42,  struct kvmfr_dmabuf_create)

int dmaFd = ioctl(rgbfd,  KVMFR_DMABUF_CREATE,  &create);  // 其他app调用ioctl KVMFR_DMABUF_CREATE //


//TODO - linux rsync

rsync -arv ./DRMTest/ \
ffjerry@10.43.48.12:/home/ffjerry/work/melo/01.ubuntu.game/0624-gamescreen/game_screen/DRMTest
source: ./DRMTest/
destnation: /home/ffjerry/work/melo/01.ubuntu.game/0624-gamescreen/game_screen/DRMTest
    a  所有文件
    r  递归
    v  verbose
    avzP  强总

rsync -a --delete ./empty/ ./wanna-delete/      /* 删除空文件夹 */
rsync -a --delete ./blank/ ./src-linux/         /* 删除大文件 */


//TODO - linux 更快删除

rsync -a --delete-before --progress --stats ~/empty target_dir/

find /path/to/directory -type f -delete
find /path/to/directory -type d -exec rmdir {} +

rm -rf --one-file-system /path/to/directory

rm -rf /path/to/directory && mkdir /path/to/directory

find /path/to/directory -type f | parallel -j 4 rm {}           /* 需要支持 parallel */
find /path/to/directory -type d | parallel -j 4 rmdir {}

//TODO - linux epoll

int epoll_ctl（int epfd, int op, int fd, struct epoll_event * event）
    op EPOLL_CTL_ADD   新加fd   EPOLL_CTL_MOD  更改fd属性   EPOLL_CTL_DEL  删除fd
    ev.data.fd   通过socket返回的sockfd
    ev.events   EPOLLIN  sockfd可读   EPOLLOUT  sockfd可写   EPOLLET  边缘触发


int epoll_wait（int epfd, struct epoll_event * events,  int maxevents, int timeout）
    epfd  被监控的fd
    timeout  超过timeout返回  如果-1 一直阻塞


//TODO - linux fstab mount

/* 自动挂载的配置文件 */
/dev/vdb1    /home/ffjerry/3d_test   xfs   defaults    0       1
1 挂载盘      2 挂载点                3 类型 4 功能选项  5 和dump相关  6 fsck检查顺序


//TODO - linux arp

/* host查找guest的ip */
arp -a


//TODO - linux awk

 awk '{print $1}' filename

echo "1:2:3" | awk -F':' '{print $3}'           // -F 分隔符 $1 1 $2 2 $3 3

//TODO - linux 重定向

使用 2> 重定向 标准错误输出信息 例如 2>/dev/null 或者 2>/home/user/error.log
使用 1> 重定向 标准输出信息
使用 &> 同时重定向 标准错误输出信息 与 标准输出信息

grep -r power /sys/ 2>/dev/null   过滤错误信息 permission denied
grep -r power /sys/ >/dev/null 2>&1   字符串 >/dev/null 代表 将标准输出重定向到 /dev/null 中 ,
    然后第二部分, 2>&1, 代表将标准错误重定向到标准输出文件中
    这里你必须将标准输出文件写成 &1, 而不是简单地写成 1
    写成 2>1 这样只会将标准输出重定向到一个名为 1 的文件中


//TODO - linux grep

grep -nr $1 \
    --include "*.h" --include "*.c" --include "*.S" --include "*.s" --include "*.py" \
    --include "*.in" --include "*.sh" --include "*.rs" --include "*.mak" --include "Makefile" \
    --include "*.whl" --include "*.xml" --include "*.js" \
    $2


//TODO - linux select API

int select(int maxfdp1,
        fd_set *readset,
        fd_set *writeset,
        fd_set *exceptset,
        struct timeval *timeout);

timeout == NULL  等待无限长的时间

timeout->tv_sec == 0 &&timeout->tv_usec == 0 不等待，直接返回

timeout->tv_sec !=0 ||timeout->tv_usec!= 0 等待指定的时间
    当有描述符符合条件或者超过超时时间的话，函数返回
    在超时时间即将用完但又没有描述符合条件的话，返回 0

readset, writeset, exceptset,指向描述符集
    这些参数指明了我们关心哪些描述符，和需要满足什么条件(可写，可读，异常)
    int FD_ZERO(int fd, fd_set *fdset);
    int FD_CLR(int fd, fd_set *fdset);
    int FD_SET(int fd, fd_set *fd_set);
    int FD_ISSET(int fd, fd_set *fdset);



//TODO - linux socket

socket() 创建socket
bind() 绑定socket到本地地址和端口，通常由服务端调用
listen() TCP专用，开启监听模式
accept() TCP专用，服务器等待客户端连接，一般是阻塞态
connect() TCP专用，客户端主动连接服务器
send() TCP专用，发送数据
recv() TCP专用，接收数据
sendto() UDP专用，发送数据到指定的IP地址和端口
recvfrom() UDP专用，接收数据，返回数据远端的IP地址和端口
closesocket() 关闭socket

``` demo
struct sockaddr_vm addr;
    doamin
        AF_VSOCK   vsock 实现vm和host之间的通信
    type -- 指明socket类型，有3种：
        SOCK_STREAM -- TCP类型，保证数据顺序及可靠性
        SOCK_DGRAM --  UDP类型，不保证数据接收的顺序，非可靠连接；
        SOCK_RAW -- 原始类型，允许对底层协议如IP或ICMP进行直接访问，不太常用。

    protocol -- 通常赋值"0"，由系统自动选择。

int socket (int domain, int type, int protocol)
addr.svm_family = AF_VSOCK;
addr.svm_port = 9999;
addr.svm_cid = VMADDR_CID_HOST;
if (connect(sockfd, (const struct sockaddr *)&addr, sizeof(struct sockaddr_vm)) < 0)
// 发送数据 //
if (send(sockfd, &msgdata, sizeof(msgdata), MSG_DONTWAIT) < 0)
    1. s 指定发送端套接字描述符。
    2. buff 表示存放发送数据的缓冲区。
    3. 实际要发送的字节数，
    4.第四个参数一般置零
```


//TODO - linux simg2img

simg2img boot.img boot-ext.img          // 将 boot 转化为 可以挂载的 boot-ext
simg2img system.img system-ext.img
sudo mount boot-ext.img b               // mount
sudo mount system-ext.img s
sudo cp $KERNEL_IMG ./b/system/boot/Image_linux         // 更新
sudo cp $KO_MODULES ./s/system/lib/modules/5.10.59/ -rf
sudo umount b                           // umount
sudo umount s
img2simg boot-ext.img boot.mod.img      // 封装 boot
img2simg system-ext.img system.mod.img

//TODO - linux watch

watch -n 0.5  cat /sys/kernel/debug/dri/0/amdgpu_pm_info   /* 0.5s 监视文件 显示变化的内容 */
watch -n 5 tail /var/log/syslog

//TODO - linux ln 软连接

ln –s  /var/www/test  ./test   /* 虚拟文件 ./test 真实文件 /var/www/test */


setenv serverip 192.168.1.1;
setenv ipaddr 192.168.1.171;
setenv bootargs "earlycon=uart8250,mmio32,0x43b90000 rdinit=/sbin/init initrd=0x95300000 debug"
setenv bootape ' \
      	tftp 0x82000000         shifan/np-bringup/linux_vm_osx_lisheng_0802.dtb; \
      	tftp 0x84000000         shifan/np-bringup/aspe.img; \
      	tftp 0x8A000000         shifan/np-bringup/Image_0801; \
        tftp 0x95300000         shifan/np-bringup/ramfs_0801.cpio; \
        tftp 0x260000000        shifan/np-bringup/init.cpio.rtos; \
        tftp 0x268000000        shifan/np-bringup/Image_rtos; \
        tftp 0x26C000000        shifan/np-bringup/rtos_kernel_cpu6.dtb
        tftp 0x210000000        shifan/np-bringup/osx_np.img; \
        tftp 0x21F000000        shifan/np-bringup/j501_osx.dtb; \
        tftp 0x220000000        shifan/np-bringup/Image_hostVM; \
        bootm 0x84000000 - 0x82000000; boot'
run bootape


//TODO - linux scp

scp -rp C:\Users\fan.shi\.ssh\id_rsa.pub shifan@10.43.201.145:/home/shifan
    从windows将文件cp到linux
    注意: scp -r 会将软连接的地址视为真正目录拷贝!!

scp -P 2222 amdgpu.ko shifan@localhost:~
    通过ssh传输

scp -P 2222 root@127.0.0.1:/1.info .
    VM打开端口转发


//TODO - linux 动态库 so

[Linux链接库通常形如libxo.so]
(http://xn--Linuxlibxo-qv8rh66b17a31fr4x1z1obnra.so)，xo就是连接时用的名字，如-lxo

[使用参数-lGL即链接当前系统中能找到的libGL.so]
(http://xn---lGLlibGL-tl6n79in0kzka90lz7a074j8ilv5e96jn58fg8fg03bcdjyxmx15j.so)

 gcc -fPIC -shared caculate.c -o [libcaculate.so]
 (http://libcaculate.so)    生成动态库

gcc -rdynamic -o main main.c -ldl    /* 编译main的bin  链接本地的 libcaculate.so */
ldd  amdgpu_drv.so                   /* 查看链接上去的其他so */
nm -D amdgpu_drv.so                  /* 查看内部符号表 */

//TODO - linux od

od -Ax -t x1 -N 52 main
/*      Ax 地址显示 hex
        Ad 地址显示 dec
        -t x1 内容显示 hex 每次1字节
        -N 读取 52字节
*/


//TODO - linux sftp

sftp -P 60608 root@30.178.39.26:/userdata
put /home/melo.sf/rpm-AUTO_pbase_C3B_RTOS_TMP/image_files/J5/rtos.img           // userdata

//TODO - linux drm /dev/cardX 查看连接的card

for p in /sys/class/drm/*/status;
    do con=${p%/status}; echo -n "${con#*/card?-}: ; cat $p; done

%/status   去除 /status
con#*/card?-   只保留card0- 之后的字符串(card?-匹配)


//TODO - linux shutdown

shutdown -r now   立刻重启


//TODO - linux 查看 发行版

/* cat /etc/*-release */

//TODO - linux fbset

fbset -g 1366 768 1920 1080 16 -t 13468 104 32 18 3 32 5

(-g xres yres vxres vyres depth)
(-t pixclock left right upper lower hslen vslen)

画面分辨率为 1366 * 768
桌面分辨率为 1920 * 1080
16表示位深，16bpp
-t 相应的timing




//TODO - linux ubuntu kernel 编译

sudo apt-get build-dep linux linux-image-$(uname -r)    ;; 安装编译内核的依赖
sudo apt-get install libncurses-dev gawk flex bison libssl-dev dkms libelf-dev libudev-dev libpci-dev \
  libiberty-dev autoconf 手动安装编译内核的依赖：
apt-get source linux-image-unsigned-$(uname -r)    ;; 获取当前运行的内核的源代码

chmod a+x debian/rules    ;; 如果用manuconfig配置, 需要添加权限
chmod a+x debian/scripts/\*
chmod a+x debian/scripts/misc/\*

LANG=C fakeroot debian/rules clean
LANG=C fakeroot debian/rules editconfigs

调用menuconfig来编辑它的配置文件。需要使用chmod，因为创建源包的方式会丢失脚本上的可执行文件位。

#清除命令
LANG=C fakeroot debian/rules clean

编译命令:
LANG=C fakeroot debian/rules binary-headers binary-generic binary-perarch

cd ..    /* 如果构建成功，将在源码根目录的上一级目录中生成.deb安装包 */
ls *.deb
sudo dpkg -i linux*4.8.0-17.19*.deb    /* 测试新kernel */
sudo reboot
sudo apt-get install pkg-config-dbgsym /* 符号表 */
LANG=C fakeroot debian/rules clean
LANG=C fakeroot debian/rules binary-headers binary-generic binary-perarch skipdbg=false

https:/\/linuxhint.com/compile-and-install-kernel-ubuntu/    /* 编译 原生kernel */
https:/\/discourse.ubuntu.com/t/how-to-compile-kernel-in-ubuntu-20-04/20268 /* 编译 kernel 生成deb-pkg */



//TODO - linux kernel atf

BL1  BootRom固化
BL2  初始化 DDR/UFS 等，并将 每一个 core 的 IMAGE 镜像加载到 DDR 指定的地址并运行
BL31 初始化 DDR/UFS 等，并将 每一个 core 的 IMAGE 镜像加载到 DDR 指定的地址并运行
BL32 安全 OS，负责运行 TEE 系统 平台使用的 TEE 是 android_trusty_tee
BL33 uboot


//TODO - linux unwind 栈回溯

/* https://blog.csdn.net/Rong_Toa/article/details/110846509 */

PC
LR
SP
FP
PARA1
PARA2
PARA3
PARA4


//TODO - kernel ifdef defined
#ifdef
只能判断单个条件


#if defined
用于多个条件判断


//TODO - linux strace

sudo strace -ff -xx -s 256 -tt -T -o test.log ./test
strace -f -p pid   查看pid的系统调用



//TODO - linux objdump

objdump -D app                                  /* -D 全部反汇编 */
objdump -S -l -z vmlinux > vmlinux.txt          /* -S 附带源码信息 */
objdump --debugging vmlinux > debugging.txt
objdump -T Xorg|grep xf86                       /* 查看xrog bin中 xf86开头的符号 */
objdump -T /bin/zsh | grep -w zfree             /* 查看zfree符号 */
aarch64-linux-gnu-objdump –s –x –d vmlinux > vmlinux.txt   /* arm64 -s 速度快 */


//TODO - 0x7c00

bios 定义的引导扇区加载的内存实模式地址

