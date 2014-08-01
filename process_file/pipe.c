/*
 * =====================================================================================
 *
 *       Filename:  pipe.c
 *
 *    Description:  管道的创建。
 *
 *        Version:  1.0
 *        Created:  2014年08月01日 09时39分50秒
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
#include <sys/types.h>
#include <string.h>

/* 读管道 */
void read_from_pipe ( int fd )
{
	char message[100];
	read ( fd, message, 100 );
	printf ( "read from pipe: %s", message );
}

/* 写管道 */
void write_to_pipe ( int fd )
{
	char *message = "Hello pipe.";
	write ( fd, message, strlen ( message ) );
}

int main(int argc, char *argv[])
{
	int 	fd[2];
	pid_t 	pid;
	int 	stat_val; 		/* 状态值 */

	/* pipe创建管道函数,成功返回0,并且数组中会有两个新的文件描述符;失败返回-1. */
	if ( pipe ( fd ) ) {
		printf ( "create pipe failed.\n" );
		exit ( 1 );
	}

	/* 创建进程 */
	pid = fork ();

	/* 管道只能用于具有亲缘关系的通信 */

	switch ( pid ) {
		case -1:
			printf ( "fork error.\n" );
			exit ( 1 );
		case 0:
			/* 子进程关闭fd1 */
			close ( fd[1] );
			read_from_pipe ( fd[0] );
			exit ( 0 );
		default:
			/* 父进程关闭 */
			close ( fd[0] );
			write_to_pipe ( fd[1] );
			wait ( &stat_val );
			exit ( 0 );

		/* 管道读端fd[0];管道写端fd[1]. */
	}

	return EXIT_SUCCESS;
}

