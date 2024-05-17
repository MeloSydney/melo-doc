


//TODO - yocto 变量
MACHINE         // 机器
BOARD           //
TOPDIR          //
DL_DIR          // download
SSTATE_DIR      // download 缓存
DISTRO          // 发行版
PACKAGE_CLASSES // 管理包的方式 rpm deb
EXTRA_IMAGE_FEATURES    //
IMAGE_FEATURES  //
PARALLEL_MAKE:pn-perl = "-j 1"          // bb任务单线程执行


{PN}    // package name expat_2.0.1.bb 将被解析为 expat
{PV}    // recipe version
{PR}    // reversion 更新PV后 PR归r0

TCMODE  // toolchain mode
EXTERNAL_TOOLCHAIN  // toolchain path


BBPATH          //
BBFILES
BBLAYERS        // meta-xxx enabled
BUILDDIR        //

BB_NUMBER_THREADS       // 多个bb任务
PARALLEL_MAKE           // 多个线程执行一个bb任务

{D}             // 组件最终安装路径
{S}             // 解压源码路径

=               // 强制
.=              // 追加
?=              // 空的情况
??=             // 如果没有 = ?= 则赋值  lazy解析

INSANE_SKIP_    // 跳过检查
PNBLACKLIST     // PN黑名单


THISDIR         //
SRC_URI += "file://defconfig"           // FILESPATH 决定路径  ${BP} or ${BPN} or files

FILESPATH       // 搜索本地文件
FILESEXTRAPATHS

BP              // ${BPN}-${PV}
BPN             // base recipe name 基于 PN 移除了 common prefixes and suffixes

WORKDIR         // ${TMPDIR}/work/${MULTIMACH_TARGET_SYS}/${PN}/${EXTENDPE}${PV}-${PR}
TMPDIR          // ${TOPDIR}/tmp  build/tmp

recipe-sysroot          // 目标机器使用
recipe-sysroot-native   // 本地编译机器使用

DEPENDS                 // 编译依赖
RDEPENDS                // 运行依赖


BBMASK += "meta-custom/recipes-example/example.bbappend"        // disable bbappend


KBUILD_DEFCONFIG        // kernel config 当使用kernel-yocto时生效

BBFILE_PATTERN_*        // 匹配 BBFILES
BBFILE_PRIORITY         // 当前layers优先级 可以用过show-layers查看


do_configure[depends] += "initramfs:do_deploy"  // config 完成之前 确保 initramfs 的 deploy已经完成
do_configure[noexec] = "1"                      // 不执行 config

//TODO - yocto 文件目录



//TODO - yocto packagegroup


packagegroup-my-custom-group.bb   // PN = packagegroup-my-custom-group

SUMMARY = "My Custom Package Group"
DESCRIPTION = "This package group includes a custom set of packages."
LICENSE = "MIT"
PR = "r1"

inherit packagegroup

RDEPENDS_${PN} = " \
        package1 \
        package2 \
        package3 \
"

IMAGE_INSTALL_append = " packagegroup-my-custom-group"   // 引用



//TODO - yocto bitbake

bitbake-layers show-layers      // 查看所有enabled的layers

bitbake-layers show-recipes     //
bitbake-layers show-recipes|grep package        //

bitbake -c clean PN
bitbake -c cleansstate PN
bitbake -c cleanall PN
bitbake -C              // 强制执行
bitbake -g core-image-c3b && cat pn-buildlist | grep package    // 解析bb依赖
bitbake -k PN           // 调试编译使用
bitbake -v              // verbose
bitbake -s              // 显示recipes版本
bitbake -p              // 只解析
bitbake -e              // 打印环境变量
bitbake -e linux-imx | grep ^SRC_URI=           // 下载地址
