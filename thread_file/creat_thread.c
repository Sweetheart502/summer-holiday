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

	printf ( "Creat thread is successful! The tid is %lu.\n", newtid );

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t 	tid;

	/* 输出主线程ID */
	printf ( "Curthread is %lu\n", pthread_self () );

	/* 创建新线程 */
	if ( pthread_create ( &tid, NULL, ( void * ) thread, NULL ) != 0 ) { 	/* 返回值非0则失败 */
		perror ( "pthread_creat" );
		exit ( 1 );
	}

	return EXIT_SUCCESS;
}

