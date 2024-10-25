nux-general.c

//TODO - tcf 语法

/* 扫描文件 */
File = ./example/example.c


/* 头文件 */
SearchPath = ./example/include

/* 宏定义 */
MacroEntry = CONFIG_EXAMPLE 1



//TODO - tcf 配置demo

# Begin Testbed Set
SET_TYPE = GROUP
SET_NAME = example
GENERATED_BY = SCRIPT

# Begin Source Files
File = ./example/example.c
# End Source Files

# Begin Sysearch Include File Entries
SearchPath = ./example/include
# End Sysearch Include File Entries

# Begin Sysppvar Preprocessor Macros
MacroEntry = CONFIG_EXAMPLE 1
# End Sysppvar Preprocessor Macros

# Begin Options
$ Options for static analysis
include = True
sys_inc_mode = 3
sys_inc_local = False
open_all_includes = False
shorten = True
cstandards_model = MISRA-C:2012/AMD1/TC1
cexternal_standard = MISRA-C:2012/AMD1/TC1
cppstandards_model = MISRA-C++:2008
cppexternal_standard = MISRA-C++:2008

$ Options for dynamic analysis
nb_substitute_source = True
nb_mechanism = build
nb_start_in_dir = ~/workspace/
nb_build_command = gcc "$$d$$f.$$e" -o "$$i$$x"
nb_compile_command = gcc -c "$$d$$f.$$e"
nb_link_command = gcc $$d$$f.o -o "$$i$$x"
# End Options

# End Testbed Set