


//TODO - kernel drm API


drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &arg); /* 通知gem 创建dumb framebuffer */
drm_mode_create_dumb(struct drm_device *dev,
				struct drm_mode_create_dumb *args,
				struct drm_file *file_priv)

DRM_IOCTL_MODE_PAGE_FLIP   drmModePageFlip   /* 基于 VSYNC 同步机制的显示刷新 */


//TODO - kernel display modeline porch

"800x600"     50      800  856  976 1040    600  637  643  666
< name >     DCF       HR  SH1  SH2  HFL     VR  SV1  SV2  VFL

pixclock: pixel clock in ps (pico seconds)
left_margin: time from sync to picture
right_margin: time from picture to sync
upper_margin: time from sync to picture
lower_margin: time from picture to sync
hsync_len: length of horizontal sync
vsync_len: length of vertical sync
Pixelclock:

xfree: in MHz
fb: in picoseconds (ps)
pixclock = 1000000 / DCF
horizontal timings:
left_margin = HFL - SH2
right_margin = SH1 - HR
hsync_len = SH2 - SH1
vertical timings:
upper_margin = VFL - SV2
lower_margin = SV1 - VR
vsync_len = SV2 - SV1


//TODO - kernel edid firmware

查看 edid firmware

cat /sys/class/drm/card0-DP-1/edid > edid.bin
hexdump -C edid.bin
modetest -M i915 -c

cat /sys/class/drm/card0-DP-1/edid | parse-edid

https://www.davejansen.com/add-custom-resolution-and-refresh-rate-when-using-wayland-gnome/



//TODO - kernel drm libdrm

libdrm

kms
	fb
	crtc
		planes
	encoder
	connector
	vblank
	property

gem
	dumb
		基于 cma  适用小分辨率
	prime
		dmabuf
	fence



//TODO - kernel dri 基本结构

1. mesa-libgl (抽象绘图API (opengl)，设备无关，user space）
2. 厂商mesa驱动（1的具体实现，user space，厂商贡献（intel）或独立（Nouveau））
3. libdrm（对4的封装，user space。如果要使用厂商扩展功能，需要厂商特定的libdrm）
4. DRM/KMS（linux kernel的抽象显卡管理API，负责统一分配显卡内存，设置显示模式等，设备无关，kernel space）
5. 厂商drm内核模块（4的具体实现，kernel space）
闭源驱动会替换1,2,5，2和5通讯是不是通过4，不知道。

估计是有额外功能。从另一个角度，厂商的闭源opengl实现也不可能去依赖一个开发过程不是完全由自己控制的开源内核模块。
说到底，也要先有闭源驱动，才能逆向工程出开源驱动。

[http://www.phoronix.com/scan.php?page=article&item=mesa-13&num=1]

DRM API不包括任何具体的绘图指令，上面的2发给5的绘图指令，每个厂商是不一样的。
同上，vulkan替代了1和2，2到5的通讯仍然要通过DRM。

source: [https://exp.newsmth.net/topic/article/b05876f5d9356cd36dc1ed6f1514823f]





//TODO - Mesa Gallium

Gallium3D架构由于backend模块独立（即Vendor HW driver 模块以及与平台窗口系统有关的WinSys 模块独立），
所以Gallium3D可以选择不同的backend，

比如上图某个Hypervisor虚拟机的方案商可以通过采用MESA-Gallium3D + virtIO-GPU backend
在Guest VM中实现虚拟GPU了功能，
**在Host VM中通过libvirglrenderer 来替Guest VM的应用响应3D GPU的绘图命令**



从上面的几个Gallium3D的时序图可以看出，对比非Gallium3D架构，
**Vendor和平台相关的实现移到了HW Vendor Driver和WinSys 模块的具体实现中**，
libmesa_dri_drivers 面向Gallium3D提供的接口实现EGL和GLES的调用，
**实现了API OS HW的三层分割**




//TODO - GLX

基于xserver的openGL实现



//TODO - EGL

EGL则是OpenGL ES在嵌入式平台上（WGL,GLX,AGL）的等价物。
EGL假设OS会提供窗口系统，但EGL与平台无关，并不局限于任何特定的窗口系统，
所有用到本地窗口系统的地方都用屏蔽指针来处理。我觉得这就是它易于移植的关键。



//TODO - GLU

OpenGL Utilities

glu是实用库，包含有43个函数，函数名的前缀为glu。
**Glu 为了减轻繁重的编程工作，封装了OpenGL函数，Glu函数通过调用核心库的函数，为开发者提供相对简单的用法**，实现一些较为复杂的操作。

//TODO - GLUT

OpenGL Utility Toolkit
glut是实用工具库，基本上是用于做窗口界面的，并且是跨平台（所以有时你喜欢做简单的demo的话，可以光用glut就ok了）



//TODO - GLEW

glew是一个跨平台的C++扩展库，基于OpenGL图形接口。使用OpenGL的朋友都知道，
window目前只支持OpenGL1.1的涵数，但 OpenGL现在都发展到2.0以上了，
要使用这些OpenGL的高级特性，就必须下载最新的扩展，
另外，不同的显卡公司，也会发布一些只有自家显卡才支 持的扩展函数，
你要想用这数涵数，不得不去寻找最新的glext.h,有了GLEW扩展库，
你就再也不用为找不到函数的接口而烦恼，因为GLEW能自动识 别你的平台所支持的全部OpenGL高级扩展涵数。
也就是说，只要包含一个glew.h头文件，你就能使用gl,glu,glext,wgl,glx的全 部函数。
GLEW支持目前流行的各种操作系统（including Windows, Linux, Mac OS X, FreeBSD, Irix, and Solaris）。



