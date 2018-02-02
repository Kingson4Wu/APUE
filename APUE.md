1. 编译源码 `make`
2. 单独编译会失败（gcc hello.c -o hello）
3. 使用apue.h和libapue.a
    - sudo cp include/apue.h /usr/include
    - sudo cp lib/libapue.a /usr/lib
    
---

+ C 头文件：`usr/include`
+ Linux 系统： `/usr/include/sys`, `/usr/include/linux`
+ C 函数库：`/lib`, `/usr/lib` (预先编译好的)

+ 预处理（.i [-E] ） --> 编译(.o [-c])(目标文件) --> 汇编（.s[-S]） --> 链接
+ -c 通常用于欧编译不包含主程序的子程序文件
+ -o 指定生成文件名

1. 分步编译（每一个.c 文件生成.o文件，然后将每一个.o链接成一个可执行文件）--- `gcc hello.o sayhello.o -o hello`
2. 一步编译（中间不生成.o）--- `gcc hello.c sayhello.c -o hello`

+ 函数库
    - 动态 .so
    - 静态 .a

+ make： 自动完成大批量源文件编译的工具
+ makefile 文件：描述如何编译（可以定义宏）
+ make工具根据makefile定义的依赖关系自底向上执行编译命令



---

### UNIX基础知识

+ 1-3 intro/ls1.c ( dirent.h ) (文件和目录)
+ 1-4 intro/mycat.c (unistd.h)（不带缓冲的IO）
    - `./mycat > kxw.txt`
    - `./mycat < kxw.txt > kxw_copy.txt`
+ 1-5 intro/getcputc.c (stdio.h)(标准IO)
+ 1-6 intro/hello.c  (unistd.h)(程序和进程)
+ 1-7 intro/shell1.c （进程控制） (TODO 为什么fork返回两次)
    - 新进程是调用进程的一个副本
    - 文件结束符（CRTL ＋ D）
+ 1-8 intro/testerror.c (出错处理)（strerror, perror）
    - `argv[0]` 程序名
+ 1-9 intro/uidgid.c (unistd.h)(用户标识)    
+ 1-10 intro/shell2.c (信号) （signal.h）
    - 键盘上产生两种信号的方法：中断键（interrupt key）- (Ctrl + C)和退出键（quit key）- (Crtl + \)
    - 调用Kill函数（该进程的用户）
    - SIGINT信号的系统默认动作是终止进程

+ 3个进程时间值：时钟时间，用户CPU时间，系统CPU时间
+ 系统调用和库函数
    - UNIX 为每个系统调用在标准C库中设置一个具有同样名字的函数。用户进程用标准调用序列来调用这些函数。
    - 从应用角度考虑，可将系统调用视为C函数。
    - 系统调用和库函数都以C函数的形式出现。

### UNIX标准化    
+ ISO C 标准定义的头文件
+ POSIX 标准定义的必需的头文件（包含IOS C）
+ 限制？？？
+ 函数sysconf, pathconf, fpathconf(运行时限制调用)
+ 2-13 standard/makeconf.awk
+ 看不太懂 TODO
+ 放弃了，后续再看

### 文件I/O
+ 不带缓冲的IO是指每个read和write函数都调用内核中的一个系统调用。
+ 当一个进程终止时，内核自动关闭它所有的打开文件。
+ 3-1 fileio/seek.c
+ 3-2 fileio/hole.c (`od  -c file.hole`)
    - ls -ls file.hole file.nohole 
    - mac os 有无空洞占用的磁盘快一样？？？？




    
    


    
    
 
    
---

+ Advanced Programming in the UNIX

+ UNIX 是一种操作系统的体系结构，Linux是GNU操作系统使用的内核名称。
+ GNU：GNU计划，又称革奴计划，是由Richard Stallman在1983年9月27日公开发起的。它的目标是创建一套完全自由的操作系统。
+ GNU操作系统是一种由自由软件构成的类 Unix 操作系统，该系统基于 Linux 内核，目标在于建立一个完全相容于UNIX的自由软件环境。

+ POSIX: POSIX表示可移植操作系统接口（Portable Operating System Interface of UNIX，缩写为 POSIX ），
POSIX标准定义了操作系统应该为应用程序提供的接口标准，是IEEE为要在各种UNIX操作系统上运行的软件而定义的一系列API标准的总称，其正式称呼为IEEE 1003，而国际标准名称为ISO/IEC 9945。
    - 为了提高 UNIX 环境下应用程序的可移植性。POSIX 已发展成为一个非常庞大的标准族，某些部分正处在开发过程中。POSIX 与 IEEE 1003 和 2003 家族的标准是可互换的。
    - Linux作为一个从头开始研制的新操作系统，逐渐发展起来以后为了尽可能获得大量应用软件支持，也明智地选择了用POSIX作为API设计的标准。
    
    
    