http://blog.csdn.net/ts173383201/article/details/7874727

[root@kingson-wu-kuiup svr]# cd unix
[root@kingson-wu-kuiup unix]# ll
total 428
drwxr-xr-x  2 root root   4096 Sep 16 08:53 tcp
drwxr-xr-x 39 root root   4096 Sep 16 09:27 unpv13e
-rw-r--r--  1 root root 427480 Sep 16 09:23 unpv13e.tar.gz
[root@kingson-wu-kuiup unix]# cd unpv13e
[root@kingson-wu-kuiup unpv13e]# cp unp.h /apps/svr/unix/tcp/
cp: cannot stat `unp.h': No such file or directory
[root@kingson-wu-kuiup unpv13e]# locate unp.h
[root@kingson-wu-kuiup unpv13e]# cd lib
[root@kingson-wu-kuiup lib]# cp unp.h /apps/svr/unix/tcp/
[root@kingson-wu-kuiup lib]# cp config.h /apps/svr/unix/tcp/
cp: cannot stat `config.h': No such file or directory
[root@kingson-wu-kuiup lib]# cd ..
[root@kingson-wu-kuiup unpv13e]# cp config.h /apps/svr/unix/tcp/

原型：extern void bzero（void *s, int n）;　　

用法：#include <strings.h>　　

功能：置字节字符串s的前n个字节为零且包括‘\0’。　　

说明：bzero无返回值，并且使用strings.h头文件，strings.h曾经是posix标准的一部分，但是在POSIX.1-2001标准里面，这些函数被标记为了遗留函数而不推荐使用。在POSIX.1-2008标准里已经没有这些函数了。推荐使用memset替代bzero。　　


http://blog.csdn.net/wangjiannuaa/article/details/6865591