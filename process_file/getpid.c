/*
 * =====================================================================================
 *
 *       Filename:  getpid.c
 *
 *    Description:  获取进程的PID。
 *
 *        Version:  1.0
 *        Created:  2014年07月21日 15时23分40秒
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
	exit ( 1 );
}

int main(int argc, char *argv[])
{
	pid_t 	pid;
	
	/* 创建新进程 */
	pid = fork ();

	if ( pid == -1 ) {
		error ( "fork", __LINE__ );
	}
	else if ( !pid ) {
		printf ( "I am the new process! My PID is %d.\n", getpid () );
	}

	return EXIT_SUCCESS;
}

