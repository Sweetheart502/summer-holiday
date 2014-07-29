/*
 * =====================================================================================
 *
 *       Filename:  creat_thread.c
 *
 *    Description:  创建一个线程。
 *
 *        Version:  1.0
 *        Created:  2014年07月24日 17时53分58秒
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
#include <pthread.h>

int * thread ( void * arg )
{
	pthread_t 	newtid;

	/* 获取新线程ID */
	newtid = pthread_self ();

	printf ( "Creat thread is successful! The newtid is %lu, the pid is %d.\n", newtid, getpid () ); 	/* 输出新线程ID */

	return NULL;
}

/* 主函数 */
int main(int argc, char *argv[])
{
	pthread_t 	tid;

	/* 输出主线程ID */
	printf ( "Curthread is %lu, the pid is %d.\n", pthread_self (), getpid () );

	/* 创建新线程 */
	if ( pthread_create ( &tid, NULL, ( void * ) thread, NULL ) != 0 ) { 	/* 返回值非0则失败 */
		perror ( "pthread_creat" );
		exit ( 1 );
	}
	
	sleep ( 1 ); 						/* 当将这行注释掉时，只输出第一行内容，不输出新的线程ID，不知道为什么 */

	return EXIT_SUCCESS;
}

