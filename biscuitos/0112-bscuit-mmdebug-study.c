

#define bs_debug(...)                                           \
({                                                              \
        if (bs_debug_kernel_enable && bs_debug_kernel_enable_one) \
                pr_info("[BiscuitOS-stub] " __VA_ARGS__);       \
})



SYSCALL_DEFINE1(debug_BiscuitOS, unsigned long, enable)
{
        if (enable == 1) {
                bs_debug_kernel_enable = 1;
                bs_debug_kernel_enable_one = 1;
        } else if (enable == 0) {
                bs_debug_kernel_enable = 0;
                bs_debug_kernel_enable_one = 0;
        } else
                bs_debug_async_data = enable;

        return 0;
}


./arch/x86/include/generated/asm/syscalls_64.h:549:__SYSCALL(548, sys_debug_BiscuitOS)

./arch/x86/include/generated/uapi/asm/unistd_64.h:366:#define __NR_debug_BiscuitOS 548


/proc/sys/BiscuitOS # RunBiscuitOS.sh
[   31.269349] [BiscuitOS-stub] melo memoryfluid 0x6000000000 on do_anonymous_page  (bs_debug)
Anonymous 0x6000000000 => B  (printf)