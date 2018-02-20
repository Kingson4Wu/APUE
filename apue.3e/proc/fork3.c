#include "apue.h"
#include <sys/wait.h>
#include <pthread.h>
#include <sys/time.h>


pthread_t ntid;

void
printids(const char *s)
{
   /* pid_t		pid;
    pthread_t	tid;

    pid = getpid();
    tid = pthread_self();
    printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid,
           (unsigned long)tid, (unsigned long)tid);*/

    //freopen("file.pid","w",stdout);
    for(;;){
        printf("pid = %ld\n", (long) getppid());
        sleep(3);
    }
}

void *
thr_fn(void *arg)
{
    printids("new thread: ");
    return((void *)0);
}

int
main(void) {
    int		err;

    err = pthread_create(&ntid, NULL, thr_fn, NULL);
    if (err != 0)
        err_exit(err, "can't create thread");
    //printids("main thread:");

    //exit(0);

    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        printf("fork success, pid = %ld\n", (long) getppid());
    }else{
        printf("main pid, pid = %ld\n", (long) getppid());
    }

    //pause();
    sleep(100);

    exit(0);
}


