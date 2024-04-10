


//TODO - linux gcc

gcc -v -o hello hello.c
gcc -g  /* 添加调试信息 */


-static  /* 编译成静态文件 */

整个过程分为三个阶段，
第一阶段编译，将c代码编译成汇编代码，使用的工具是cc1
第二阶段是汇编，将汇编代码汇编成二进制程序，使用的工具是as
第三阶段是链接，将hello.o对象文件同基本库对象文件链接，生成可执行的elf程序，使用的工具是collect2



//TODO - linux cc1


cc1 -quite hello.c -quite -dumpbase hello.c -mtune=generic -march=x86-64 -auxbase hello -version -o hello.S

