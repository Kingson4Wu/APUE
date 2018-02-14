#include "apue.h"

int
main(int argc, char *argv[])
{
	int		status;

	if (argc < 2)
		err_quit("command-line argument required");

	if ((status = system(argv[1])) < 0)
		err_sys("system() error");

	pr_exit(status);

	exit(0);
}

//gcc ../lib/error.c ../lib/prexit.c systest3.c -o tsys
//./tsys ./pruids
//sudo chown root tsys
//sudo chmod u+s tsys
//ll tsys
//mac os 不生效。。。
