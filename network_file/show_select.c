/*
 * =====================================================================================
 *
 *       Filename:  show_select.c
 *
 *    Description:  observing this kind of the functions.
 *
 *        Version:  1.0
 *        Created:  2014年08月03日 19时50分24秒
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
#include <sys/time.h>
#include <time.h>

void display_time ( const char *string )
{
	int 		seconds;

	seconds = time ( ( time_t * ) NULL );

	printf ( "%s, %d\n", string, seconds );
}

int main(int argc, char *argv[])
{
	fd_set 		readfds;
	struct timeval 	timeout;
	int 		ret;

	/* 监视文件描述符0是否有数据输入，文件描述符0表示标准输入，即键盘输入 */
	FD_ZERO ( &readfds );
	FD_SET ( 0, &readfds );

	/* wait up to ten seconds */
	timeout.tv_sec = 10; 			/* seconds */
	timeout.tv_usec = 0; 			/* microseconds */

	while ( 1 ) {
		display_time ( "before select" );

		/* 多路复用 select ()
		 * C/S 模型中，服务器端需要同时处理多个客户端的连接请求。
		 * 参数readfds指定需要监视的可读文件描述符集合，当这个集合中的一个描述符上有数据到达时，系统将通知调用select函数的程序。
		 * 发生事件，则返回发生事件的文件描述符的个数；否则返回0.
		 */

		ret = select ( 1, &readfds, NULL, NULL, &timeout );
		display_time ( "after select" );

		switch ( ret ) {
			case 0:
				printf ( "No data in ten seconds.\n" );
				exit ( 0 );
				break;
			case -1:
				perror ( "select" );
				exit ( 1 );
				break;
			default:
				getchar (); 			/* 将数据读入，否则标准输入将一直读数据 */
				printf ( "Data is available now.\n" );
		}
	}

	return EXIT_SUCCESS;
}

