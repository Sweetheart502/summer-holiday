/*
 * =====================================================================================
 *
 *       Filename:  order_process.c
 *
 *    Description:   展示父子进程是交替执行的。
 *
 *        Version:  1.0
 *        Created:  2014年07月18日 18时26分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sweetheart502, liyingxiao502@gmail.com
 *        Company:  Class 1304 of Software Engineering
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

void error ( char * err_string, int line )
{
	fprintf ( stderr, "line: %d\n", line );
	perror ( err_string );
	exit ( -1 );
}

int main(int argc, char *argv[])
{
	pid_t 	pid;
	char 	* info;
	int 	k = 0;

	/* 调用fork函数创建进程 */
	pid = fork ();

	/* 根据返回值的不同，执行不同的语句 */
	switch ( pid ) {
		case 0:
			info = "Child process is running.";
			k = 3;
			break;
		case -1:
			error ( "fork", __LINE__ );
			break;
		default :
			info = "Parent process is running.";
			k = 5;
			break;
	}

	while ( k > 0 ) {
		puts ( info );
		sleep ( 1 );
		k--;
	}

	/*
	 * 从输出结果上可以看出，父子进程交替进行。
	 * 子进程与父进程有一些不同的属性，主要如下：
	 * 	1.子进程有它自己唯一的进程ID 
	 * 	2.fork的返回值不同，父进程返回子进程的ID,子进程返回0
	 * 	3.不同的父进程ID.子进程的父进程ID为创建它的父进程ID
	 * fork在创建失败时，返回-1.失败原因通常是：父进程拥有的子进程数超过规定的限制。
	 */

	return EXIT_SUCCESS;
}

