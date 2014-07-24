/*
 * =====================================================================================
 *
 *       Filename:  wait.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年07月24日 09时09分59秒
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
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	pid_t 	pid;
	char 	* info;
	int 	k;
	int 	exit_code;

	printf ( "Study how to get exit code!\n" );
	pid = fork ();

	switch ( pid ) {
		case 0:
			info = "Could process is running.";
			k = 5;
			exit_code = 37;
			break;
		case -1:
			perror ( "fork" );
			exit ( 1 );
		default:
			exit_code = 0;
			break;
	}
	/* 父进程都会执行以下这段代码，子进程pid值为0，父进程pid值为子进程的id */
	if ( pid ) { /* 父进程等待子进程结束 */
		int 	stat_val;
		pid_t 	child_pid;

		child_pid = wait ( &stat_val );

		printf ( "Child process has exited, pid = %d\n", child_pid );
		if ( WIFEXITED ( stat_val ) )
		      printf ( "Child exited with code %d\n", WEXITSTATUS ( stat_val ) );
		else 
		      printf ( "Child exited abnormally\n" );
	}
	else { /* 子进程暂停５秒，在这个过程中可以运行命令ps aux 查看父进程状态。 */
		while ( k-- > 0 ) {
			puts ( info );
			sleep ( 5 );
		}
	}

	return exit_code;
}

