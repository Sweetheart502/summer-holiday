/*
 * =====================================================================================
 *
 *       Filename:  orphan_process.c
 *
 *    Description:  孤儿进程：一个子进程的父进程先于子进程结束，子进程就成为一个孤儿进程（由init收养）。
 *
 *        Version:  1.0
 *        Created:  2014年07月21日 11时33分47秒
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

int main(int argc, char *argv[])
{
	pid_t pid; 				//fork函数的返回值

	/* 调用fork函数创建一个新进程 */
	pid = fork (); 	

	switch ( pid ) {
		case -1:
			error ( "fork", __LINE__ );
			break;
		case 0:
			while ( 1 ) { 		/* 用一个死循环来体现在父进程终止后，子进程成为孤儿进程，进而由init进程收养，即其父进程PID变为１ */
				printf ( "I am child process!\nMy PID is %d, my parent PID is %d.\n\n", getpid ( ), getppid ( ) );
				sleep ( 3 );
			}
		default :
			printf ( "I am parent process!\nMy child PID is %d, my PID is %d.\n\n", pid, getpid ( ) );
			/* 用exit函数终止该进程 */
			exit ( 0 ); 
	}

	return EXIT_SUCCESS;
}

