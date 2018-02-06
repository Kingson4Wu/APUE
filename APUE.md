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

### 1.UNIX基础知识

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

### 2.UNIX标准化    
+ ISO C 标准定义的头文件
+ POSIX 标准定义的必需的头文件（包含IOS C）
+ 限制？？？
+ 函数sysconf, pathconf, fpathconf(运行时限制调用)
+ 2-13 standard/makeconf.awk
+ 看不太懂 TODO
+ 放弃了，后续再看

### 3.文件I/O
+ 不带缓冲的IO是指每个read和write函数都调用内核中的一个系统调用。
+ 当一个进程终止时，内核自动关闭它所有的打开文件。
+ 3-1 fileio/seek.c （设置偏移无IO操作）
+ 3-2 fileio/hole.c (`od  -c file.hole`)
    - `ls -ls file.hole file.nohole`
    - mac os 有无空洞占用的磁盘快一样？？？？
+ 3-5 fileio/mycat.c
+ IO的效率：缓冲区设置为4096
+ 文件共享
    - 内核使用3种数据结构表示打开文件。
        1. （进程表）每个进程在进程表都有一个记录项，记录项中包含一张打开文件描述符表（fd标志：文件指针（指向文件表项））
        2. （文件表）内核为所有打开文件维持一张文件表（文件状态标志：当前文件偏移量，v节点指针）。
        3. （v节点表）每个打开文件（或设备）都有一个v节点（v-node）结构（v节点信息，vdata。。。）。（linux没有，类似实现）
    - 在fork发生后，父进程和子进程各自的每一个打开文件描述符共享同一个文件表项。
+ 原子操作
    - 追加文件使用O_APPEND
    - 使用pread和pwrite
    - 创建文件使用O_CREAT和O_EXCL
+ 复制文件描述符（dup，dup2，fcntl）
    - 使不同文件描述符指向同一个文件表项
    - 两个进程各自打开一个文件则会产生两个文件表项
+ sync,fsync,fdatasync
+ 3-11 fileio/fileflags.c (`./fileflags 2 2>>temp.foo`)
+ 3-12 fileio/setfl.c
+ `set_fl(STDOUT_FILENO, O_SYNC);` 开启同步写标志（会增加系统时间和时钟时间）（在Linux可能不起作用）
+ 函数ioctl：IO操作杂物箱
+ `/dev/fd` 目录    
    
### 4.文件和目录
+ 4个stat函数
+ 七种文件类型（普通文件，目录，块文件，字符文件，FIFO，socket，链接文件）
+ 4-3 filedir/filetype.c (`./filetype /dev/tty /dev/fd/2 /dev/disk0 /dev/stderr `)
+ 设置用户ID和设置组ID 
+ 进程关联的id
    1. 实际用户ID，实际组ID：我们实际上是谁。（取自当前登录会话中口令文件的登录项）
    2. 有效用户ID，有效组ID，附属组ID：用于文件访问权限检查。（默认是1（实际），若3有设置，取3的值）
    3. 保存的设置用户ID，保存的设置组ID：有exec函数保存。（执行一个程序时2的副本，在文件模式字st_mode中设置）
        - 如何设置，命令: `chown root file`, `chmod u+s file` 打开设置用户ID位
        - st_mode中设置一个特殊标志：当执行此文件时，将进程的有效用户ID设置为文件所有者的用户ID（st_uid）
        - 若文件所有者是超级用户，而且设置了该文件的设置用户ID位，那么当该程序文件由一个进程执行时，该进程具有超级用户权限。不管执行文件的进程的实际ID是什么。
+ 文件的访问权限：读r，写w，执行x；用户u，组g，其他o。
    - 目录执行权限位（搜索位）
    - 目录：r -> 读目录项，w -> 在目录删除和创建文件，x -> 在目录中搜索给定路径
    - S_ISVTX 粘着位 -> 限制在目录中删除和重命名文件
+ 新文件的用户ID设置为进程的有效用户ID。Linux下新文件的组ID优先取该目录的设置组ID位，若没有则取进程的有效组ID。
+ 内核以进程的有效用户ID和有效组ID为基础执行其访问权限测试。
   - 进程内部的逻辑可以根据需求验证其实际用户能访问一个给定的文件。
   - 函数access和faccessat是按实际用户ID和实际组ID进行访问权限测试的。
+ 4-8 filedir/access.c   
+ 函数umask。为进程设置文件模式屏蔽字。使任何用户都能读写文件，将umask设置为0.
+ 4-9 filedir/umask.c 
+ log4j
```xml
<File name="File" fileName="my.log" filePermissions="rw-r--r--">
   <PatternLayout pattern="%m%n" />
</File>
```
+ 函数chmod，fchmod，fchmodat
+ 粘着位：为目录的文件模式设置粘着位可以使改目录下的文件任何人都可以读写执行，但不可以删除或重命名属于其他人的文件。！！！
+ 函数chown,fchown,fchownat,lchown
+ 检查文件中的空洞
    - `du -s file`. sum * 512 .Linux若设置了POSIXLY_CORRECT变量，则是1024. st_blocks(512字节)
    - 对于没有写过的字节位置，read函数读到的字节是0. `wc -c file` 显示正常I/O操作整个文件的长度
    - 使用cat复制文件，空洞会被填满。`cat file > file.copy`    
+ 文件截断：函数ftruncate
+ 文件系统（硬链接，软链接（符号链接））
+ 函数link, linkat, unlink, unlinkat, remove
+ 函数rename, renameat
+ 文件的时间 (i节点中的所有信息跟文件的实际内容是分开存放的)
    - st_atime 文件数据的最后访问时间 （read）
    - st_mtime 文件数据的最后修改时间（write）
    - st_ctime i节点状态的最后更改时间（chmod, chown, ln）
    - Linux中用Stat查看文件的3个时间点
+ 函数futimens, utimensat, utimes . 修改一个文件的访问和修改时间
+ 4-21 filedir/zap.c (mac上不支持，文件时间的数据类型变了。linux未试)
+ 函数mkdir, mkdirat, rmdir
+ 读目录（dirent.h）
+ 4-22 filedir/ftw8.c (统计各种类型的文件计数)（七种文件类型）`./ftw8 .`
+ 函数chdir, fchdir, getcwd
+ 设备特殊文件
+ 文件访问权限位小结

### 5.标准I/O库
+ 不仅是UNIX，很多其他操作系统都实现了标准I/O库。
+ 在第3章中，所有的I/O函数都是围绕着文件描述符的。当打开一个文件时，及返回一个文件描述符，然后该文件描述符就用于后续的I/O操作。
而对于标准I/O库，它们的操作是围绕流（Stream）进行的。
+ 标准I/O文件流可用于单字节或多字节（"宽"）字符集。fwide
+ 缓冲：标准I/O库提供缓冲的目的是尽可能减少使用read和write调用的次数。
    - 全缓冲，行缓冲，不带缓冲
+ 标准I/O库最终还是要调用第三章的例程。
+ 按系统默认，流被打开时是全缓冲的。若流引用终端设备，则该流是行缓冲的。打开流之后，在对流进行任何操作之前，可以使用setbuf或setvbuf改变缓冲类型。
+ 标准I/O自动为流分配适当的长度的缓冲区。由常量BUFSIZ所指定的值（1024）。
+ 5-12 stdio/temofiles.c
+ 标准I/O库把数据缓存到内存中，因此每次一字符和每次一行的I/O更有效。
+ 标准I/O的替代软件
    - 标准I/O库并不完善。
    - 标准I/O库的不足之处是效率不搞。
    - fgets和fputs需要两次拷贝，内核与标准I/O缓冲区，标准I/O缓冲区与用户程序中的行缓冲区。
    - sfio
    - mmap函数
    - nClibc, Newlib
    
### 6.系统数据文件和信息
+ 口令文件`/etc/passwd`, 组文件`/etc/group` , 都是ASCII文本文件
+ 超级用户root，用户ID是0
+ nobody
+ 禁止登录设置为/bin/false或/bin/true
+ 函数getpwuid, getpwnam (pwd.h)
+ 6-2 datafiles/getpwnam.c
+ 加密口令字段包含一个占位符x，加密口令字符存放在另一个文件。（shadow password 阴影口令）（shadow.h）
+ 加密口令是经单向加密算法处理过的用户口令副本。
+ 少数几个需要访问加密口令，如login和passwd，这些程序需要设置用户ID为root！！！！
+ 组文件（grp.h）
+ 附属组ID
    - 每个用户任何时候都只属于一个组。
    - 但是用户可以设置多个组ID作为其附属组ID
    - 用户不仅可以属于口令文件记录项中组ID所对应的组，也可以属于多至16个另外的组。
    - 文件访问权限检查为：不仅将进程的有效组ID与文件的组ID项比较，而且也将所有附属组ID与文件的组ID进行比较。！！！
    - 如何为用户设置附属组ID ： TODO
+ Linux 和Solaris支持阴影口令文件。FreeBSD和Mac OS X 则以不同方式存储加密口令字。
+ 主机 /etc/hosts (netdb.h)
+ 记录各网络服务器所提供服务的数据文件/etc/services(netdb.h)
+ 记录协议信息的数据文件/etc/protocols(netdb.h)
+ 记录网络信息的数据文件/etc/networks(netdb.h)
+ 登录账户记录：utmp, wtmp
+ 系统标识：uname
+ 时间和日期time_t(内容可后续查阅)

---

### 7.进程环境


    
    

---
   
+ atoi (表示 ascii to integer)是把字符串转换成整型数的一个函数   
    


    
    
 
    
---

+ Advanced Programming in the UNIX

+ UNIX 是一种操作系统的体系结构，Linux是GNU操作系统使用的内核名称。
+ GNU：GNU计划，又称革奴计划，是由Richard Stallman在1983年9月27日公开发起的。它的目标是创建一套完全自由的操作系统。
+ GNU操作系统是一种由自由软件构成的类 Unix 操作系统，该系统基于 Linux 内核，目标在于建立一个完全相容于UNIX的自由软件环境。

+ POSIX: POSIX表示可移植操作系统接口（Portable Operating System Interface of UNIX，缩写为 POSIX ），
POSIX标准定义了操作系统应该为应用程序提供的接口标准，是IEEE为要在各种UNIX操作系统上运行的软件而定义的一系列API标准的总称，其正式称呼为IEEE 1003，而国际标准名称为ISO/IEC 9945。
    - 为了提高 UNIX 环境下应用程序的可移植性。POSIX 已发展成为一个非常庞大的标准族，某些部分正处在开发过程中。POSIX 与 IEEE 1003 和 2003 家族的标准是可互换的。
    - Linux作为一个从头开始研制的新操作系统，逐渐发展起来以后为了尽可能获得大量应用软件支持，也明智地选择了用POSIX作为API设计的标准。
    
    
    