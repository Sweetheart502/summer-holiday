/*
 * =====================================================================================
 *
 *       Filename:  execve.c
 *
 *    Description:  演示exec函数的用法，这里以execve为例。
 *
 *        Version:  1.0
 *        Created:  2014年07月21日 17时06分50秒
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

int main(int argc, char * argv[], char ** environ)
{
	pid_t pid;

	pid = fork ();

	if ( pid == -1 ) {
		perror ( "fork" );
		exit (1);
	}
	else if ( !pid ) {
		printf ( "I am child process!\n" );
		printf ( "My PID is %d, my parent PID is %d.\n", getpid (), getppid () );
		printf ( "The uid is %d, the gid is %d", getuid (), getgid () );
		
		execve ( "image", argv, environ );

		printf ( "It cannot come here.\n" );
		exit ( 0 );
	}
	else {
		printf ( "Parent process is running.\n" );
	}

	return 0;
}

