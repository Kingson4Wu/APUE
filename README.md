<http://blog.csdn.net/ts173383201/article/details/7874727/>

### sockaddr和sockaddr_in (socket.h )
<http://blog.csdn.net/joeblackzqq/article/details/8258693>
1. sockaddr是在头文件 /usr/include/bits/socket.h 中定义的
2. sockaddr_in是在头文件 /usr/include/netinet/in.h

+ 二者的占用的内存大小是一致的，因此可以互相转化，从这个意义上说，他们并无区别。
sockaddr常用于bind、connect、recvfrom、sendto等函数的参数，指明地址信息。是一种通用的套接字地址。而sockaddr_in 是internet环境下套接字的地址形式。所以在网络编程中我们会对sockaddr_in结构体进行操作。使用sockaddr_in来建立所需的信息，最后使用类型转化就可以了。

---
两个函数 htons() 和 inet_addr()。
htons()作用是将端口号由主机字节序转换为网络字节序的整数值。(host to net)
inet_addr()作用是将一个IP字符串转化为一个网络字节序的整数值，用于sockaddr_in.sin_addr.s_addr。
inet_ntoa()作用是将一个sin_addr结构体输出成IP字符串(network to ascii)。比如：
printf("%s",inet_ntoa(mysock.sin_addr));


htonl()作用和htons()一样，不过它针对的是32位的，而htons()针对的是两个字节，16位的。
与htonl()和htons()作用相反的两个函数是：ntohl()和ntohs()。

INADDR_ANY就是inet_addr("0.0.0.0")
当服务器的监听地址是INADDR_ANY时，那就监听所有的

### socket编程为什么要选择AF_INET
<http://dwz.cn/35GemO>
AF_INET（又称 PF_INET）是 IPv4 网络协议的套接字类型，AF_INET6 则是 IPv6 的；而 AF_UNIX 则是 Unix 系统本地通信。
选择 AF_INET 的目的就是使用 IPv4 进行通信。因为 IPv4 使用 32 位地址，相比 IPv6 的 128 位来说，计算更快，便于用于局域网通信。
而且 AF_INET 相比 AF_UNIX 更具通用性，因为 Windows 上有 AF_INET 而没有 AF_UNIX。

### SOCK_STREAM vs SOCK_DGRAM
SOCK_STREAM提供面向连接的稳定数据传输，即TCP协议。SOCK_STREAM应用在C语言socket编程中，在进行网络连接前，需要用socket函数向系统申请一个通信端口。
SOCK_DGRAM 是无保障的面向消息的socket，主要用于在网络上发广播信息。SOCK_DGRAM是基于UDP的，专门用于局域网，基于广播,SOCK_DGRAM分是数据包,是UDP协议网络编程

### socket 函数
int socket(int domain, int type, int protocol);
在参数表中，domain指定使用何种的地址类型，比较常用的有：
PF_INET, AF_INET： Ipv4网络协议；
PF_INET6, AF_INET6： Ipv6网络协议。
type参数的作用是设置通信的协议类型，可能的取值如下所示：
SOCK_STREAM： 提供面向连接的稳定数据传输，即TCP协议。
OOB： 在所有数据传送前必须使用connect()来建立连接状态。
SOCK_DGRAM： 使用不连续不可靠的数据包连接。
SOCK_SEQPACKET： 提供连续可靠的数据包连接。
SOCK_RAW： 提供原始网络协议存取。
SOCK_RDM： 提供可靠的数据包连接。
SOCK_PACKET： 与网络驱动程序直接通信。
参数protocol用来指定socket所使用的传输协议编号。这一参数通常不具体设置，一般设置为0即可。

### bzero和memset区别
bzero
　　原型：`extern void bzero（void *s, int n）;`　　
用法：#include <string.h>　　功能：置字节字符串s的前n个字节为零且包括‘\0’。　
说明：bzero无返回值，并且使用strings.h头文件，strings.h曾经是posix标准的一部分，
但是在POSIX.1-2001标准里面，这些函数被标记为了遗留函数而不推荐使用。在POSIX.1-2008标准里已经没有这些函数了。推荐使用memset替代bzero。
`void *memset(void *s, int ch, size_t n);`　　
函数解释：将s中前n个字节替换为ch并返回s

### time_t (time.h )
time_t实际上是长整型
<http://www.cnblogs.com/mydomain/archive/2013/06/02/3113720.html>

###  c语言中time函数的用法
<http://blog.csdn.net/wangluojisuan/article/details/7045592>

<http://ganquan.info/standard-c/function/fputs>
<http://ganquan.info/standard-c/>

### VIM自动格式化C代码
<http://leloup.blog.51cto.com/1113857/398128>


<http://www.360doc.com/content/12/0925/15/7534118_238097045.shtml>









---
<http://blog.csdn.net/ts173383201/article/details/7874727/>
第一种：TCP迭代服务器程序：迭代服务器总是在完全处理了一个客户的请求后才响应下一个客户的请求。
gcc daytimetcpsvr.c -o server -lunp
gcc daytimetcpcli.c -o client -lunp

run:
./server
./client 127.0.0.1

第二种：TCP并发服务器程序,每个客户一个子进程
每个客户一个子进程：传统上，并发服务器调用fork派生一个子进程来处理每个客户。这使得服务器可在同一时间为多个客户提供服务。

Signal(SIGCHLD, sig_chld);是为了捕捉信号SIGCHLD并处理僵尸进程，有关僵尸进程的介绍请大家参看博文unix网络编程之基本套接口编程while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0),这里我们不能用wait，由于wait会阻塞于没有子进程终止的情况所有不能用while循环每个终止信号。

第三种：使用单进程和select/poll的TCP服务器程序


---
+ 现在我们可以对后 C 时代的未来窥见一斑，即上述这类领域的代码都可以使用那些具有强大内存安全特性的 C 语言的替代者实现。Go 、Rust 或者 Cx ，无论是哪个，都可能使 C 的存在被弱化。比如，如果我现在再来重新实现一遍 NTP ，我可能就会毫不犹豫的使用 Go 语言去完成。
+ 与 C 语言长别离 | Linux 中国:<https://mp.weixin.qq.com/s/pdDuD7hphCxkMcOe8mmF9g> 

---

+ git-oschina/IT-Book/BOOK/Unix
+ https://github.com/VVictorQi/APUE





