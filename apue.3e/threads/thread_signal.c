#include "apue.h"
#include <pthread.h>

static void	sig_usr(int);	/* one handler for both signals */
static void	sig_usr2(int);

pthread_t ntid;

void
printids(const char *s)
{
	pid_t		pid;
	pthread_t	tid;

	pid = getpid();
	tid = pthread_self();
	printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid,
	  (unsigned long)tid, (unsigned long)tid);
}

void *
thr_fn(void *arg)
{
	if (signal(SIGUSR1, sig_usr2) == SIG_ERR)
		err_sys("can't catch SIGUSR1");
	if (signal(SIGUSR2, sig_usr2) == SIG_ERR)
		err_sys("can't catch SIGUSR2");
	for ( ; ; )
		pause();
}

static void
sig_usr(int signo)		/* argument is signal number */
{
	if (signo == SIGUSR1)
		printf("received SIGUSR1\n");
	else if (signo == SIGUSR2)
		printf("received SIGUSR2\n");
	else
		err_dump("received signal %d\n", signo);
}

static void
sig_usr2(int signo)		/* argument is signal number */
{
	if (signo == SIGUSR1)
		printf("received SIGUSR1-2\n");
	else if (signo == SIGUSR2)
		printf("received SIGUSR2-2\n");
	else
		err_dump("received signal %d\n", signo);
}


int
main(void)
{
	int		err;

	err = pthread_create(&ntid, NULL, thr_fn, NULL);
	if (signal(SIGUSR1, sig_usr) == SIG_ERR)
		err_sys("can't catch SIGUSR1");
	for ( ; ; )
		pause();
}

//gcc thread_signal.c ../lib/error.c -o thread_signal


