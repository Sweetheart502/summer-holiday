/*
 * =====================================================================================
 *
 *       Filename:  creat_process.c
 *
 *    Description:  创建一个子进程，并分析fork函数的返回值情况。
 *
 *        Version:  1.0
 *        Created:  2014年07月21日 09时57分41秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sweetheart502, liyingxiao502@gmail.com
 *        Company:  Class 1304 of Software Engineering
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

void error ( char * err_string, int line )
{
	fprintf ( stderr, "line: %d\n", line );
	perror ( err_string );
	exit ( -1 );
}

	/*
	 * fork函数创建当前进程的一份拷贝。
	 * 当前进程成为父进程，新进程成为子进程。
	 * 返回值：成功时，在父进程中返回子进程ID,在子进程中返回0;
	 * 	　 失败时，返回值为-1.
	 * pid=-1: 当前进程数已达到系统规定的上限，errno值为EAGAIN;
	 * 	   系统可提供使用的内存不足，errno值为ENOMEM.
	 * 创建成功后，进程的执行顺序取决于内核所使用的调度算法。
	 */

int main(int argc, char *argv[])
{
	pid_t pid;

	/*
	 * getpid函数获取进程ID
	 * getppid函数获取父进程ID
	 */

	/* 调用函数创建子进程 */
	pid = fork ();

	if ( pid == -1 ) { 
		/* 创建进程失败 */
		error ( "fork", __LINE__ );
	}
	else if ( !pid ) {
		/* 创建进程成功，当前返回值是０，所以当前进程为子进程 */
		printf ( "\tI am the child process!\n\tThe returned PID is %d, my parent's ID is %d.\n\n", pid, getppid ( ) );
	}
	else {
		/* 进程创建成功，当前返回值为子进程ID，当前进程为父进程 */
		printf ( "\tI am the parent process!\n\tMy child's PID is %d, my PID is %d.\n\n", pid, getpid ( ) );
	}

	return EXIT_SUCCESS;
}

