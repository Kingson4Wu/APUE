+ http://csapp.cs.cmu.edu/3e/students.html
---
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

#### 动态库和静态库的区别
+ <https://zhidao.baidu.com/question/1946953913764139388.html>
+ 我们通常把一些公用函数制作成函数库，供其它程序使用。
  　　函数库分为静态库和动态库两种。
  　　静态库在程序编译时会被连接到目标代码中，程序运行时将不再需要该静态库。
  　　动态库在程序编译时并不会被连接到目标代码中，而是在程序运行是才被载入，因此在程序运行时还需要动态库存在。
+ 无论静态库，还是动态库，都是由.o文件创建的。`gcc -c hello.c`
+ 静态库文件名的命名规范是以lib为前缀，紧接着跟静态库名，扩展名为.a。创建静态库用ar命令。`ar cr libmyhello.a hello.o`
    - 用gcc命令生成目标文件时指明静态库名，gcc将会从静态库中将公用函数连接到目标文件中。注意，gcc会在静态库名前加上前缀lib，然后追加扩展名.a得到的静态库文件名来查找静态库文件。
    `gcc -o hello main.c -L. -lmyhello`
+ 动态库文件名命名规范和静态库文件名命名规范类似，也是在动态库名增加前缀lib，但其文件扩展名为.so。
    - `gcc -shared -fPCI -o libmyhello.so hello.o`
    - `gcc -o hello main.c -L. -lmyhello` (程序在运行时，会在/usr/lib和/lib等目录中查找需要的动态库文件。若找到，则载入动态库，否则将提示类似上述错误而终止程序运行。我们将文件 libmyhello.so复制到目录/usr/lib中，再试试。)
    




---

### 1.UNIX基础知识
+ 内核的接口被称为系统调用。公用函数库构建在系统调用接口之上，应用程序既可使用公用函数库，也可以使用系统调用。
shell是一个特殊的应用程序，为运行其他程序提供一个接口。

+ 1-3 intro/ls1.c ( dirent.h ) (文件和目录)
+ 1-4 intro/mycat.c (unistd.h)（不带缓冲的IO）
    - `./mycat > kxw.txt`
    - `./mycat < kxw.txt > kxw_copy.txt`
+ 1-5 intro/getcputc.c (stdio.h)(标准IO)
+ 1-6 intro/hello.c  (unistd.h)(程序和进程)
+ 1-7 intro/shell1.c （进程控制） (为什么fork返回两次：进程副本，看下面其他笔记的解释)
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
    - 如何为用户设置附属组ID
        - <http://blog.163.com/chujb_chen/blog/static/105711120160270337976/>
        - 从用户的角度，分为主组和附属组。
          主组：也被称为primary group、first group或initial login group，用户的默认组，用户的gid所标识的组。
          附属组：也被称为Secondary group或supplementary group，用户的附加组。
        - 通过id命令可查看当前用户的主组和附属组: gid标识主组，groups表示用户所属的全部组（主组和附属组）
        - 当通过useradd命令创建新用户时，可以通过-g参数指定已存在的某个组为其主组，若没有使用-g参数，
          则系统自动创建名称和用户名相同的组作为该用户的主组
        - 通过usermod -G 设置普通用户的附属组。-g后面的组是用户的主组，-G后面的组是用户的附加组  
         

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
+ main函数
+ 进程终止
    1. 正常终止（5种）
    2. 异常终止（3种）
    - main函数中exit(0) 等价于 return(0)
    - 使用atexit函数登记终止处理程序，一个进程可登记多至32个。调用顺序与登记顺序相反。
+ 内核使程序执行的唯一方法是调用一个exec函数。进程自愿终止的唯一方法是显示或隐式地调用exit。非自愿终止由一个信号使其终止。
+ 7-3 environ/doatexit.c 
+ 7-4 environ/echoarg.c (命令行参数)
+ 每个程序都接收一张环境表。environ指针。
+ C程序的存储空间布局
    1. 正文段。由CPU执行的机器指令部分。
    2. 初始化数据段。
    3. 未初始化数据段。
    4. 栈。
    5. 堆。
+ 需要存放在磁盘文件中的段只有正文段和初始化数据段。
+ 共享库。共享库使得可执行文件中不再需要包含公用的库函数，而只需在所有进程都引用的存储区中保存这种库例程的一个副本。
程序第一次调用某个库函数时，用动态链接方法将程序与共享库函数相链接。
+ 存储空间分配: malloc, calloc, realloc
    - 函数free释放指针ptr指向的存储空间。忘记调用free会使该进程占用的存储空间连续增加，导致泄漏（leakage）!!!
+ 替代的存储空间分配程序
    1. libmalloc
    2. vmalloc
    3. quick-fit
    4. jemalloc !!
    5. TXMalloc
    6. alloca
+ 环境变量（getenv, putenv, setenv, unsetenv）
+ 函数setjmp, longjmp: goto语句不能跨越函数，而执行这种类型跳转功能。
这两个函数对于处理发生在很深层嵌套函数调用中的出错情况是非常有用的。
+ 7-9 environ/cmd1.c
+ 7-11 environ/cmd2.c
+ 在希望返回的位置调用setjmp，在触发返回的地方调用longjmp。暂不继续详看
+ 每个进程都有一组资源限制，其中一些可以用getrlimit和setrlimit函数查询和更改。
+ 资源限制影响到调用进程并由子进程继承。为了影响一个用户的所有后续进程，需将资源限制的设置构造在shell之中。shell具有内置的ulimit命令。
+ 7-16 environ/getrlimit.c


### 8. 进程控制
+ 进程ID是唯一的，可复用的。大多数UNIX系统实现延迟复用算法。
+ ID为0的进程通常是调度进程，常被称为交换进程。该进程是内核的一部分，它不执行任何磁盘上的程序，也称系统进程。
+ 进程ID为1的通常是init进程，在自举过程结束时由内核调用。/etc/init或/sbin/init。此进程负责在自举内核后启动一个UNIX系统。
init通常读取与系统有关的初始化文件（/etc/rc*文件或/etc/inittab文件，/etc/init.d）。init进程绝不会终止。它是一个普通用户进程，以超级用户特权运行。
init时所有孤儿进程的父进程！！！
+ 某些UNIX，进程ID2是页守护进程，此进程负责支持虚拟存储器系统的分页操作。
+ 函数fork
    - fork函数被调用一次，但返回两次。子进程返回值是0，而父进程返回是新建子进程的进程ID。
    - 没有一个函数是一个进程可以获得其所有子进程的进程ID。
    - 子进程可以通过getppid获取其父进程的进程ID。
    - 子进程是父进程的副本。子进程获得父进程数据空间，堆和栈的副本。父进程和子进程并不共享这些存储空间部分。父进程和子进程共享正文段。
    - 如今很多实现并不执行一个父进程数据段、堆和栈的完全副本。而使用写时复制(Copy-On-Write，COW)技术。
    这些区域由父进程和子进程共享，而内核将它们的访问权限改变为只读。 !!!
    - Linux 3.2.0 提供另一种新进程创建函数-clone系统调用。允许调用者控制哪些部分由父进程和子进程共享。
+ 父进程和子进程谁先执行是不确定的。    
+ 8-1 proc/fork1.c
    - ./fork1 > fork1.txt
    - printf 指的是打印到标准输出。标注输出可以是控制台（行缓冲）。也可以是文件（全缓冲）。
    - fork 子进程，父进程的缓冲区也会被复制。
    - 文件共享： 父进程的所有文件描述符都被复制到子进程中。同执行dup函数。父进程和子进程每个相同的打开描述符共享一个文件表项。
+ strlen计算不包含终止null字符。sizeof包含。strlen进行一次函数调用，sizeof是在编译时计算缓冲区长度，长度是固定的。   
+ CHILD_MAX 每个实际用户ID在任一时刻可拥有的最大进程数。
+ fork 的两种用法：
    1. 一个父进程希望复制自己，使父进程和子进程通知执行不同的代码段。（网络服务进程，父进程监听，子进程处理）
    2. 一个进程要执行一个不同的程序。子进程从fork返回之后立即调用exec
+ vfork函数用于创建一个新进程，而该新进程的目的使exec一个新程序（fork的第二种用法）
    - vfork和fork的另一个区别是：vfork保证子进程先运行，在它调用exec或exit之后父进程才可能被调度运行。
+ 8-3 proc/vfork1.c    
+ exit会执行标准I/O缓冲区的冲洗操作，_exit则不会。
    
+ 函数exit
    - 进程有5种正常终止及3种终止方式。
        1. 在main函数内执行return语句。
        2. 调用exit函数（其操作包括调用各终止处理了程序，关闭所有标准I/O流等）
        3. 调用_exit或_Exit函数。（无2中括号里的操作）（！！！大多数UNIX系统实现中，exit事标准C库中的一个函数，而_exit则是一个系统调用）
        4. 进程的最后一个线程在其启动例程中执行return语句。
        5. 进程的最后一个线程调用pthread_exit函数。
        1. 调用abort。它产生SIGABRT信号，这是下2中的一种特例。
        2. 当进程接收某些信号时。
        3. 最后一个线程对"取消"请求作出响应。
    - !!!! 不管进程如何终止，最后都会执行内核中的同一段代码。为相应的进程关闭所有打开描述符，释放它所使用的存储器等。
    - 在任意一种情况下，该终止进程的父进程都能用wait或waitpid函数取得其终止状态。
    - 最后调用_exit是，内核将退出状态（传递给终止函数的参数，或main返回值）转换成终止状态。    
    - 对于父进程已经终止的所有进程，它们的父进程都改变为init进程。！！
    - 内核为每个终止子进程保存了一定量的信息，当终止进程的父进程调用wait或waitpid时，可以等到这些信息。
    - ！！！在UNIX术语中，一个已经终止、但是其父进程尚未对其进行善后（获取终止子进程的有关信息、释放它仍占有的资源）的进程称为僵死进程（zombie）。ps（Z）
    （如果编写一个长期运行的程序，它fork了很多子进程，除非父进程等待取得子进程的终止状态，否则这些子进程最终会成为僵死进程）
    需要自己调用wait函数释放资源？？（某些系统提供一种避免僵死进程的方法10.7）
    - 只有一个init的子进程终止，init就会调用一个wait函数取得其终止状态。防止系统塞满僵死进程。
+ 函数wait和waitpid
    - 当一个进程正常或异常终止时，内核就向其父进程发生SIGCHLD信号（异步事件）。父进程可以提供一个被调用执行得函数，或者忽略它。系统默认时忽略。
    - 如果进程由于接收到SIGCHLD信号而调用wait，应该立即返回。否则调用wait，进程可能会阻塞。
    - 终止状态用定义在<sys/wait.h>的各个宏查看。
    - 8-5 lib/prexit.c
    - 8-6 proc/wait1.c
    - wait和waitpid的区别和用法（祥看书）
    - 8-8 proc/fork2.c (父进程不等待子进程终止，但有不想子进程处于僵死状态。调用fork两次，使子进程的父进程是init进程)！！！！
+ 函数waitid，类似waitpid，提供更多的灵活性。
+ 函数wait3和wait4，可以返回终止进程及其所有子进程使用的资源概况。
+ 竞争条件：不加控制的情况下，依赖于系统负载以及内核的调度算法。
    - 为了避免竞争条件和轮询，在多个进程之间需要有某种形式的信号发生和接收的方法。在UINX中可以使用信号机制。或者各种形式的IPC。 
    - 8-12 proc/tellwait1.c 
    - 8-13 proc/tellwait2.c (信号实现)
+ 函数exec(!!!)
    - 当进程调用一种exec函数时，该进程执行的程序完全替换成新程序。调用exec函数不创建新进程，进程ID不变。
    - exec只是用磁盘上的一个新程序替换了当前进程的正文段、数据段、堆段和栈段。
    - 有7种不同的exec函数供使用。(用法祥看书)
    - ！！！UNIX系统进程控制原语：用fork可以创建新进程，用exec可以初始执行新的程序。exit函数和wait函数处理终止和等待终止。
    - 进程中的每个打开描述符都有一个执行时关闭标志。若设置了该标志，则在执行exec时关闭该描述符。通常opendir函数会调用fcntl函数关闭目录流的描述符设置该标志。
    - !!! 在exec前后实际用户ID和实际用户组ID保持不变，而有效ID是否改变取决于所执行程序文件的设置用户ID位和设置组ID位是否设置。
    - 8-16 proc/exec1.c
+ 更改用户ID和更改组ID
    - 最小特权模型。
    - 用setuid函数设置实际用户ID和有效用户ID。
+ 解释器文件
    - 对这种文件的识别是有内核作为exec系统调用处理的一部分来完成。内核使调用exec函数的进程实际执行的并不是该解释器文件，而是
    在该解释器文件第一行中所致的文件。
    - 8-20 proc/exec2.c
    - 如果不能向解释器传递至少一个可选参数（如 awk中的-f），那么这些解释器文件只对shell才是有用的。！！！
+ 函数system
    - system在其实现中调用了fork、exec、和waitpid，对应3种返回值。
    - 8-22 proc/system.c    
    - 8-23 proc/systest1.c
    - 使用system而不是直接使用fork和exec的有点是：system进行了所需的各种出错处理以及各种信号处理。
    - !!!设置用户ID程序中不应该调用system，这会导致安全性漏洞！！！（mac os貌似没成功）
    - 给予tsys程序的超级权限在system中执行了fork和exec之后被保持下来。
    - 8-24 proc/systest3.c
    - system函数使用（execl函数）212页，使用/bin/sh（可以方便字动分割参数） 执行。（可以通过设置IFS造成漏洞）
    - 有些实现更改/bin/sh，当有些用户ID与实际用户ID不匹配时，将有效用户ID设置成实际用户ID，这样关闭安全漏洞。！！！！（避免使用system函数造成的漏洞）
    - !!! 如果一个进程以特殊权限运行，需要生成另一个进程，不应该直接调用system函数。应该直接使用fork和exec，在fork之后exec之前改回普通权限。
+ 进程会计
    - 启动选项之后，每当进程结束时内核就会写一个会计记录。
+ 用户标识
    - 一个人在口令文件中可以有多个登录项，它们的用户ID相同，但登录shell不同。
    - getlogin函数失败，则是守护进程daemon
+ 进程调度
    - UNIX系统对进程提供基于调度优先级的粗粒度的控制。调度策略和调度优先级由内核确定。进程通过调整nice值选择以更低的优先级运行。nice值越高，优先级越低（"友好"）。
    - 默认值NZERO（mac os  20）
    - 进程可以通过nice函数调整
    - 子进程从父进程中继承nice值。
    + nice 值对于java线程？？
        - 线程总是存在优先级，优先级范围在1~10之间。JVM线程调度程序是基于优先级的抢先调度机制。在大多数情况下，当前运行的线程优先级将大于或等于线程池中任何线程的优先级。但这仅仅是大多数情况。
        - 注意：当设计多线程应用程序的时候，一定不要依赖于线程的优先级。因为线程调度优先级操作是没有保障的，只能把线程优先级作用作为一种提高程序效率的方法，但是要保证程序不依赖这种操作。
        - 当线程池中线程都具有相同的优先级，调度程序的JVM实现自由选择它喜欢的线程。这时候调度程序的操作有两种可能：一是选择一个线程运行，直到它阻塞或者运行完成为止。二是时间分片，为池内的每个线程提供均等的运行机会。
        - 设置线程的优先级：线程默认的优先级是创建它的执行线程的优先级。可以通过setPriority(int newPriority)更改线程的优先级。
        - java线程优先级实现原理(搜不到结果。。。)
        - nice值是 进程优先级，跟线程没关系。。。。。！！！！
+ 进程时间（3个度量时间）：通过调用times函数获得自身及已终止子进程的值。

+ java 创建子进程的原理 
    - proc/fork3.c, 子进程没有继承父进程的定时任务？？!!
    - java进程调用外部程序时fork()+exec()分析
      <pre>
      java进程调用一个外部程序，一般使用Runtime.getRuntime().exec(cmd)的方式启动。
      
      以下是Runtime.getRuntime().exec(cmd) 的执行流程分析
      
      分析SUN JDK 1.5 SRC，找到Runtime.getRuntime().exec(cmd)的执行流程：
      
      java.lang.Runtime.exec(cmd);
      
      --java.lang.ProcessBuilder.start();
      
      ----java.lang.ProcessImpl.start();
      
      ------Java_java_lang_UNIXProcess_forkAndExec() in j2se/src/solaris/native/java/lang/UNIXProcess_md.c
      
      --------1). fork(); 2). execvp();
      
      man fork知道，fork产生的子进程需要复制父进程在内存中的所有数据内容（代码段、数据段、堆栈段），由于全部复制开销较大，因此Linux已经采用copy-on-write机制，即只是复制页表，共享内容，在有改变的时候再去申请内存和复制数据。
      
      虽然Linux早已在fork()中采用copy-on-write机制，但是JVM调
      用fork()后，Java进程里的其它线程往往会被调度回来继续执行，修改了自己的内存，而这个时候
      execvp()还没有执行，于是悲剧就发生了，内存都要重新复制一遍。
      
      (在内核侧，在进行了内存“复制”后，子进程与父进程指向同一个只读的Page分页。当子进程或者父进程发送修改内存请求后，由于是分页是只读的，OS此时才将内存进行复制为两份，并将这两份内存设置为可写权限，最后再处理刚刚发送的修改内存请求。)
      (COW技术：
       Linux的fork()使用写时复制技术，父子进程用的是相同的物理空间（内存区），子进程的代码段、数据段、堆栈都是指向父进程的物理空间(也就是说，两者的虚拟空间不同，但其对应的物理空间相同)。
       fork()的实际开销就是复制父进程的页表以及给子进程创建惟一的进程描述符。
       当父子任一进程试图修改数据段、堆栈，才会为修改的区域(通常为虚存中一页)制作副本。)
      
      参考：
      
      http://my.oschina.net/jsan/blog/273672
      </pre>
      - 可以单独运行一个Java程序，称为JavaB吧，由它负责调用外部程序，JavaA调用我们封装后的接口与之通信，等待外部程序结束，从而与 Runtime.getRuntime().exec(cmd) 的语义保持一致。这个单独运行的JavaB只需要很小很小的内存，因此不太可能出现无法分配内存，进而无法执行外部程序的问题了。
    - Java中Runtime.getRuntime().exec()错误：Cannot allocate memory
      - 此bug是由于在JDK1.6以前版本，Runtime.exec调用外部程序时使用fork()方式，需要分配和当前Java主进程同等大小的内存空间，也就是说将当前占用内存加倍。因此当Java程序占用内存超过50%时执行Runtime.exec永远不能成功。
        解决方法：
        1、新起一个Java进程，通过socket与主进程通信。这样运行Runtime.exec时只会将新进程内存加倍。
        2、坐等JDK1.7发布。该bug在JDK1.7中被Fix。
    - 执行Runtime.exec异常: error=12,Cannot allocate memory:<http://blog.csdn.net/chifengxin/article/details/6573134>
      - Description
        If you run a "small" program (e.g., a Perl script) from a "big" Java process on
        a machine with "moderate" free swap space (but not as much as the big Java
        process), then Runtime.exec() fails.
      - Cause Detail:
        This issue dues to the operating mechanism of “Runtime.exec” in Java.
        In Java program, “ProcessBuilder.start” and “Runtime.exec” use fork() on *NIX system, which allocates the child process the same amount of memory as the parent process. This will double the used memory for a short time. So when the Java main process has used over 50% memory, it will absolutely never launch a child process using “Runtime.exec” successful, even the process needs almost no memory.  
      - There are three workable solutions:
        1. The middleware of Tanuki may solve this problem. The question is, it is complicated and also expensive.
            See-  http://wrapper.tanukisoftware.com/doc/english/child-exec.html
        2. Separate the process using “Runtime.exec” from the main process into a new java process. So when the “Runtime.exec” is called ,it will only double the memory of the new process, using almost no memory.
        which means:
            a. When start/stop the main process, the “Runtime.exec” process should be started/stopped at the same time.
            b. Add an independent socket in the main process to communicate with the new process. The exec command will be delivered to the new process to execute.
        This will surely increase the complexity and maintenance of the system.
        3. Update JDK1.6 to JDK1.7
        This bug is fixed in JDK1.7 ,using new invoking mechanism of external program.
        (Pipes be tested in JDK snapshot release: build 1.7.0-b147,passed with no exceptions; it also reduced the memory usage for about 20%)
        This will need no modification for current source code.
      - Use posix_spawn, not fork, on S10 to avoid swap exhaustion
        - 在main函数中调用该函数可以将一个可执行文件运行起来
        - <http://www.embedu.org/Column/8020.html>
        <pre>
        根据posix_spawn函数原型和功能可知，针对fork函数创建子进程时复制的主要三个方面有如下的方法替代：
        
        1. 复制文件描述符表：posix_spawn函数自身在创建子进程时也复制父进程所有的文件描述符，父子进程的文件描述符表指向同一个文件结构，这个和fork函数一样；
        
        2. 复制数据段：posix_spawn函数可以通过第五个参数指针数组argv[ ]进行数据传递，对于对父进程的代码段的复制只能通过再次编写来解决；
        
        3. 复制坏境变量：posix_spawn函数可以使用第六个参数数指针数组envp[ ]来传递父进程的环境参数，并且可以认为设置envp[ ]参数来设置子进程的环境参数。
        
        注：因为在SylixOS父子进程因为是共享同一张页表的，所以对于SylixOS操作系统的父子进程天生就有共享内存（即共享同一张页表地址），不过需要注意如果父子进程通过使用同一张页表同一个地址来传递数据很危险，需谨慎使用（比如子进程到共享页表地址进行读写操作，不小心踩到父进程的堆栈空间会造成不可估量的危险）。
        </pre>
        - 父子进程因为是共享同一张页表
      - fork vs. posix_spawn Sun:<http://blog.csdn.net/maimang1001/article/details/29625153>  
      - <http://www.man7.org/linux/man-pages/man3/posix_spawn.3.html>
        - The posix_spawn() and posix_spawnp() functions are used to create a
               new child process that executes a specified file.  These functions
               were specified by POSIX to provide a standardized method of creating
               new processes on machines that lack the capability to support the
               fork(2) system call.  These machines are generally small, embedded
               systems lacking MMU support.
        
        - The posix_spawn() and posix_spawnp() functions provide the
               functionality of a combined fork(2) and exec(3), with some optional
               housekeeping steps in the child process before the exec(3).  These
               functions are not meant to replace the fork(2) and execve(2) system
               calls.  In fact, they provide only a subset of the functionality that
               can be achieved by using the system calls.
    - http://hg.openjdk.java.net/jdk8u/jdk8u60/jdk/file/935758609767/src/solaris/native/java/lang/UNIXProcess_md.c           
    - posix_spawn is the way to go (vfork is removed in modern POSIX standard,
      so it should be avoided when possible).
    - There is one reason to not use vfork. It's when the child needs to dup a
      FUSE-backed file descriptor, which could block, and in the case of vfork,
      also block the parent for indefinite amount of time.  

### 9.进程关系
+ 终端登录
    - 终端窗口（仿真了基于字符的终端，使得用户可以用熟悉的方式（shell命令）与主机进行交互
+ 网络登录
    - 网络登录时，在终端和计算机之间的连接不再是点到点的。在网络登录情况下，login仅仅是一种可用的服务，这与其他网络服务（FTP，SMTP）性质相同。
    - 伪终端
+ 进程组
    - 每个进程除了有一个进程ID之外，还属于一个进程组。
    - 进程组是一个或多个进程的集合。同一个进程组中的各进程接受来自同一终端的各种信号。对应唯一的进程组ID。
    - 每个进程组有一个组长进程。组长进程的进程ID等于其进程ID。
    - 进程调用setpgid可以创建或加入一个进程组。
+ 会话
    - 会话session是一个或多个进程组的集合。通常由shell的管道将几个进程编成一组。
    - 调用setsid函数建立一个新会话。（进程组的组长）
+ 控制终端
    - 建立与控制终端连接的会话首进程被称为控制进程。
    - 一个会话中的几个进程组可被分成一个前台进程组以及一个或多个后台进程组。
    - 函数tcgetpgrp，tcsetpgrp，tcgetsid。通知或设置内核哪个是前台进程组。
+ 作业控制
    - 在一个终端上启动多个作业（进程组），控制哪一个作业可以访问该终端以及哪些作业后台进行。
    - 键入回车，shell通知作业完成
    - 挂起键（Ctrl + Z）:键入此字符使终端驱动程序将信号SIGTSTP发送至前台进程组中的所有进程（后台进程不受影响）。
    - 三个特殊字符使终端驱动程序产生信号
        1. 中断字符(Ctrl + C 或 Delete) ： SIGINT
        2. 退出字符(Ctrl + \)：SIGQUIT
        3. 挂起字符（Ctrl + Z）：SIGTSTP
    - 将后台作业转成前台作业运行：`fg %1`
    - 禁止后台作业输出到控制终端 
+ shell 执行程序（再详看及下面）
+ 孤儿进程组
+ FreeBSD实现

### 10.信号
+ 信号是软件中断。信号提供一种异步事件的方法。
+ 每个信号名字以SIG开头。
+ <signal.h> 定义
+ kill 函数对信号编号0（空信号）有特殊应用。
+ 信号的产生条件
    1. 用户按某些终端键。
    2. 硬件异常产生信号。
    3. 进程使用kill函数发生信号给另一个进程或进程组。
    4. 用户使用kill命令（kill函数的接口）
    5. 检测到某种软件条件发生，通知有关进程。
+ 信号出现，内核3中处理方式
    1. 忽略该信号（SIGKILL和SIGSTOP不能被忽略）
    2. 捕捉信号（调用用户函数，不能捕捉SIGKILL和SIGSTOP）
    3. 执行系统默认动作。（大多数是终止该进程）
+ 函signal（后期sigaction替代）
    - 10-2 signals/sigusr.c (pause函数，使调用进程在接受到一信号之前挂起) `./sigusr &`,
    - `kill -USR1 pid`, `kill -USR2 pid`, `kill pid`
+ 信号SIGTERM：由kill命令发送的系统默认终止信号。该信号由应用程序捕获，让程序优雅终止程序。默认动作是终止。
+ 1. 程序启动：exec函数将原本要捕捉的信号更改位系统默认。
  2. 进程创建：调用fork函数，其子进程继承父进程的信号处理方式。
+ 中断系统的调用（早期）：低速系统调用被信号中断之后不会继续执行。
+ 可重入函数：  除了可重入函数，在信号处理操作期间，会阻塞任何会引起不一致的信号发送。
+ 10-5 signals/reenter.c (mac os 运行有问题)
+ SIGCLD（Linux上等同于SIGCHLD）
+ 当一个信号产生时，内核通常在进程表以某种形式设置一个标志。
+ 每个进程都有一个信号屏蔽字，它规定了当前要阻塞递送到该进程的信号集。
+ 函数sigsuspend
    - 等待一个信号处理程序设置一个全局变量，用信号实现父、子进程之间的同步
+ 用户定义信号USER1，USER2
+ 函数abort
+ 信号这章以后再看。。。。TODO
    
### 11.线程
+ 多个进程必须使用操作系统提供的复杂机制才能实现内存和文件描述符的共享。
+ 进程数据类型pid_t，线程数据类型pthread_t
+ 都是使用函数（暂不看TODO）
    
### 12.线程控制
    
+ 信号处理是进程中所有线程共享的。（貌似不能重复接受同个信号）    
+ threads/thread_signal.c (线程信号不能重复用？？)
+ 暂不看TODO

### 13.守护进程
+ 通常由系统引导装入时启动，仅在系统关闭时才终止，没有控制终端，后台运行。
+ `ps -axj` -x 没有控制终端，-j 显示与作业有关的信息， -a 显示其他用户所拥有的进程
+ PPID 父进程ID，PGID 进程组ID，SID 会话ID，TTY 终端名称
+ 系统进程依赖于操作系统实现。父进程ID为0的各进程通常是内核进程，作为系统引导装入过程的一部分而启动。
+ 进程1通常是init（Mac OS X中是launchd）
+ 内核守护进程，用户级守护进程（父进程是init）。
+ 如何编写守护进程374页（调用fork，使父进程exit，调用setsid等）
+ 13-1 daemons/init.c 初始化一个守护进程
+ 出错记录：集中守护进程出错记录。（syslog）（/etc/syslog.conf）
    - 内核进程调用log函数；用户进程（守护进程）调用syslog函数；将日志消息发向UDP端口514（不产生数据报）.
+ 单实例守护进程：任一时刻只允行该守护进程的一个副本。
+ 13-6 daemons/single.c (只允行该守护进程的一个副本)（这个例子有点搓，根本不是长期运行的守护进程，只是实现关键逻辑）
+ 守护进程的惯例（382页）
    - 若守护进程使用锁文件，那么文件通常存储在/var/run目录中。
+ 13-7 daemons/reread.c (守护进程重读配置文件)
+ SIGHUP信号： 连接断开（守护进程通常捕捉该信号重读配置文件）！！！！！
+ 客户进程-服务器进程模型（如nginx）
    - 守护进程常常用作服务器进程。
    - 在服务器进程中调用fork然后exec另一个程序来向客户进程提供服务是很常见的。
+ 设置执行时关闭标志！
    

### 14.高级I/O
+ 非阻塞I/O
    - 14-1 advio/nonblockw.c(长的非阻塞write) `./nonblockw < /etc/services > temp.file`
+ 记录锁
    - 当一个进程正在读或修改文件的某个部分时，使用记录锁可以阻止其他进程修改同一文件区。
    - UNIX系统叫字节范围锁（byte-range locking），它只锁定文件的一个区域。
    - fcntl函数
    - 各种锁（不详细看TODO）
+ I/O多路转接
    - 函数select和pselect
    - 函数poll
    - 异步I/O
    - readv和writev函数
    - 存储映射I/O（memory-mapped I/O）（能将一个磁盘文件映射到存储空间中的一个缓冲区上，于是，当从缓冲区中读数据时，就相当于读文件中的相应字节）
    - 不详细看TODO

### 15.进程间通信
+ InterProcess Communication, IPC
+ 管道、FIFO、消息队列、信号量、共享存储
+ 管道
    - 半双工
    - 只能在具有公共祖先的两个进程之间使用。
    - pipe函数
    - 通常进程会先调用pipe，接着fork，从而创建从父进程到子进程的IPC通道。
    - 15-5 ipc1/pipe1.c(经由管道从父进程向子进程传送数据)
    - 15-6 ipc1/pipe2.c(将文件复制到分页程序)（UNIX系统公用程序具有分页功能）
    - 15-7 ipc1/tellwait.c(让父进程和子进程同步的例程)
    - 函数popen和pclose：创建一个管道，fork一个子进程，关闭未使用的管道端，执行一个shell运行命令，然后等待命令终止。
+ 协同进程
+ FIFO
    - 有时被称为命名管道。通过FIFO，不相关的进程也能交换数据。
    - mkfifo,mkfifoat
+ XSI IPC: 消息队列、信号量、共享存储器。
    - 每个内核中的IPC结构（消息队列、信号量、共享存储段）都有一个非负整数的标志符（identifier）加以引用。
+ 消息队列
    - 消息队列是消息的链接表，存储在内核中，由消息队列标识符标识。
    - msgget,msgctl,msgsnd
+ 信号量
+ 共享存储
    - 共享存储允许两个或多个进程共享一个给定的存储区。因为数据不需要在客户进程和服务器进程之间复制，这是一种最快的IPC。
+ POSIX信号量(提供替代信号量机制)
+ 在新的应用程序中，尽可能避免使用消息队列以及信号量，应当考虑全双工管道和记录锁。共享存储仍有用途，通过mmap可以提供同样的功能。
    
### 16.网络IPC：套接字
+ (管道、FIFO、消息队列、信号量、共享存储)，这些机制允许在同一台计算机上运行的进程可以相互通信。不同计算机（通过网络连接）上的
    进程相互通信的机制：网络进程间通信（network IPC）
+ 套接字网络进程间通信接口，进程用该接口能够和其他进程通信。
+ <sys/socket.h>
+ TCP/IP协议栈使用大端字节序。
+ <arpa/inet.h>  htonl函数。h表示"主机"字节序，n表示"网络"字节序，l表示"长"整数（4字节），s表示"短"整数（2字节）。
+ /etc/services 
    - /etc/services文件是记录网络服务名和它们对应使用的端口号及协议。文件中的每一行对应一种服务，它由4个字段组成，中间用TAB或空格分隔，分别表示“服务名称”、“使用端口”、“协议名称”以及“别名”。
    - /etc/services文件包含了服务名和端口号之间的映射，很多的系统程序要使用这个文件。一般情况下，不要修改该文件的内容，因为这些设置都是Internet标准的设置。一旦修改，可能会造成系统冲突，使用户无法正常访问资源。
    - <http://blog.chinaunix.net/uid-30414403-id-5190762.html>
+ 带外数据（紧急数据）TCP。
+ 没仔细看TODO

### 17.高级进程间通信
+ 高级IPC——UNIX域套接字机制：在同一计算机系统上运行的两个进程之间传送打开文件描述符。
+ UNIX域套接字用于同一台计算机上运行的进程之间的通信。因特网域套接字也可用于同一目的，但UNIX域套接字的效率更高。
UNIX域套接字仅仅复制数据，不执行协议处理，添加网络包头等。。。
+ UNIX域套接字提供流和数据报两种接口。UNIX域套接字就像套接字和管道的混合。
+ 没仔细看TODO

### 18.终端I/O
+ TODO

### 19.伪终端
+ expect 程序
+ TODO

### 20.数据库函数库
+ 访问数据库的函数库通常使用两个文件来存储信息：一个索引文件和一个数据文件。
+ TODO

### 21.与网络打印机通信
+ 网络打印协议（IPP）为建立基于网络的打印系统指定了通信规则。IPP是建立在标准的因特网协议上的。
+ TODO

### 附录A 函数原型
### 附录B 其他源代码
### 附录C 部分习题答案



    
    


---

### 轻量级进程
+ <http://blog.csdn.net/handsomehong/article/details/73928873>
+ 一直以来, linux内核并没有线程的概念. 每一个执行实体都是一个task_struct结构, 通常称之为进程. 如图2.
+ 进程是一个执行单元, 维护着执行相关的动态资源. 同时, 它又引用着程序所需的静态资源.
通过系统调用clone创建子进程时, 可以有选择性地让子进程共享父进程所引用的资源. 这样的子进程通常称为轻量级进程. 
+ linux上的线程就是基于轻量级进程, 由用户态的pthread库实现的.
使用pthread以后, 在用户看来, 每一个task_struct就对应一个线程, 而一组线程以及它们所共同引用的一组资源就是一个进程.
+  一组线程并不仅仅是引用同一组资源就够了, 它们还必须被视为一个整体.
  对此, POSIX标准提出了如下要求:
  1, 查看进程列表的时候, 相关的一组task_struct应当被展现为列表中的一个节点;
  2, 发送给这个"进程"的信号(对应kill系统调用), 将被对应的这一组task_struct所共享, 并且被其中的任意一个"线程"处理;
  3, 发送给某个"线程"的信号(对应pthread_kill), 将只被对应的一个task_struct接收, 并且由它自己来处理;
  4, 当"进程"被停止或继续时(对应SIGSTOP/SIGCONT信号), 对应的这一组task_struct状态将改变;
  5, 当"进程"收到一个致命信号(比如由于段错误收到SIGSEGV信号), 对应的这一组task_struct将全部退出;
  6, 等等(以上可能不够全);
          
#### linux threads
+  在linux 2.6以前, pthread线程库对应的实现是一个名叫linuxthreads的lib. 
     linuxthreads利用前面提到的轻量级进程来实现线程, 但是对于POSIX提出的那些要求, linuxthreads除了第5点以外, 都没有实现(实际上是无能为力):
     1, 如果运行了A程序, A程序创建了10个线程, 那么在shell下执行ps命令时将看到11个A进程, 而不是1个(注意, 也不是10个, 下面会解释);
     2, 不管是kill还是pthread_kill, 信号只能被一个对应的线程所接收;
     3, SIGSTOP/SIGCONT信号只对一个线程起作用;          
+ 线程的创建与销毁都是通过管理线程来完成的, 于是管理线程就成了linuxthreads的一个性能瓶颈. 
  创建与销毁需要一次进程间通信, 一次上下文切换之后才能被管理线程执行, 并且多个请求会被管理线程串行地执行.
      
#### NPTL
+ 到了linux 2.6, glibc中有了一种新的pthread线程库--NPTL(Native POSIX Threading Library). 
  NPTL实现了前面提到的POSIX的全部5点要求. 但是, 实际上, 与其说是NPTL实现了, 不如说是linux内核实现了.
+ 在linux 2.6中, 内核有了线程组的概念, task_struct结构中增加了一个tgid(thread group id)字段. 
  如果这个task是一个"主线程", 则它的tgid等于pid, 否则tgid等于进程的pid(即主线程的pid).
  在clone系统调用中, 传递CLONE_THREAD参数就可以把新进程的tgid设置为父进程的tgid(否则新进程的tgid会设为其自身的pid).
  类似的XXid在task_struct中还有两个：task->signal->pgid保存进程组的打头进程的pid、task->signal->session保存会话打头进程的pid。通过这两个id来关联进程组和会话。

#### NGPT

+ 说到这里, 也顺便提一下NGPT(Next Generation POSIX Threads). 
上面提到的两种线程库使用的都是内核级线程(每个线程都对应内核中的一个调度实体), 这种模型称为1:1模型(1个线程对应1个内核级线程);
而NGPT则打算实现M:N模型(M个线程对应N个内核级线程), 也就是说若干个线程可能是在同一个执行实体上实现的. 
线程库需要在一个内核提供的执行实体上抽象出若干个执行实体, 并实现它们之间的调度. 这样被抽象出来的执行实体称为用户级线程.
大体上, 这可以通过为每个用户级线程分配一个栈, 然后通过longjmp的方式进行上下文切换. (百度一下"setjmp/longjmp", 你就知道.)
但是实际上要处理的细节问题非常之多. 
目前的NGPT好像并没有实现所有预期的功能, 并且暂时也不准备去实现.        
        
+ 用户级线程的切换显然要比内核级线程的切换快一些, 前者可能只是一个简单的长跳转, 而后者则需要保存/装载寄存器, 进入然后退出内核态. (进程切换则还需要切换地址空间等.)
  而用户级线程则不能享受多处理器, 因为多个用户级线程对应到一个内核级线程上, 一个内核级线程在同一时刻只能运行在一个处理器上.
  不过, M:N的线程模型毕竟提供了这样一种手段, 可以让不需要并行执行的线程运行在一个内核级线程对应的若干个用户级线程上, 可以节省它们的切换开销.
  
+  !!! 用户级线程就是绿色线程，协程？？！！！
  

---
+ <https://www.cnblogs.com/qquan/p/4321666.html>
+ 进程是资源管理的最小单位，线程是程序执行的最小单位。在操作系统设计上，从进程演化出线程，最主要的目的就是更好的支持SMP以及减小（进程/线程）上下文切换开销。
+ 针对线程模型的两大意义，分别开发出了核心级线程和用户级线程两种线程模型，分类的标准主要是线程的调度者在核内还是在核外。前者更利于并发使用多处理器的资源，而后者则更多考虑的是上下文切换开销。

+ linux线程的实现
: <https://www.cnblogs.com/zhaoyl/p/3620204.html>
    
    

---
   
+  atoi (表示 ascii to integer)是把字符串转换成整型数的一个函数   
+ linux下进程的进程最大数、最大线程数、进程打开的文件数和ulimit命令修改硬件资源限制:<http://blog.csdn.net/gatieme/article/details/51058797>
    - 使用ulimit -a查看我们系统的所有限制
    


    
    
 
    
---

+ Advanced Programming in the UNIX

+ UNIX 是一种操作系统的体系结构，Linux是GNU操作系统使用的内核名称。
+ GNU：GNU计划，又称革奴计划，是由Richard Stallman在1983年9月27日公开发起的。它的目标是创建一套完全自由的操作系统。
+ GNU操作系统是一种由自由软件构成的类 Unix 操作系统，该系统基于 Linux 内核，目标在于建立一个完全相容于UNIX的自由软件环境。

+ POSIX: POSIX表示可移植操作系统接口（Portable Operating System Interface of UNIX，缩写为 POSIX ），
POSIX标准定义了操作系统应该为应用程序提供的接口标准，是IEEE为要在各种UNIX操作系统上运行的软件而定义的一系列API标准的总称，其正式称呼为IEEE 1003，而国际标准名称为ISO/IEC 9945。
    - 为了提高 UNIX 环境下应用程序的可移植性。POSIX 已发展成为一个非常庞大的标准族，某些部分正处在开发过程中。POSIX 与 IEEE 1003 和 2003 家族的标准是可互换的。
    - Linux作为一个从头开始研制的新操作系统，逐渐发展起来以后为了尽可能获得大量应用软件支持，也明智地选择了用POSIX作为API设计的标准。
    
    
    