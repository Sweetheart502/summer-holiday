/*
 * =====================================================================================
 *
 *       Filename:  condition.c
 *
 *    Description:  条件变量的应用代码。
 *
 *        Version:  1.0
 *        Created:  2014年07月31日 10时29分36秒
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
#include <pthread.h>

pthread_mutex_t 	mutex; /* 锁 */
pthread_cond_t 		cond;  /* 条件变量 */

void * thread1 ( void * arg )
{
	pthread_cleanup_push ( pthread_mutex_unlock, &mutex );

	while ( 1 ) {
		printf ( "thread1 is running.\n" );

		pthread_mutex_lock ( &mutex ); 		/* 加锁 */
		pthread_cond_wait ( &cond, &mutex ); 	/* 等待 */
		printf ( "thread1 applied the condition.\n" );
		pthread_mutex_unlock ( &mutex ); 	/* 解锁 */
		
		sleep ( 4 );
	}

	pthread_cleanup_pop ( 0 );
}

void * thread2 ( void * arg )
{
	while ( 1 ) {
		printf ( "thread2 is running.\n" );

		pthread_mutex_lock ( &mutex );
		pthread_cond_wait ( &cond, &mutex );
		printf ( "thread2 applied the condition.\n" );
		pthread_mutex_unlock ( &mutex );

		sleep ( 1 );
	}
}

int main(int argc, char *argv[])
{
	pthread_t 	tid1, tid2;

	/* 锁的初始化 */
	pthread_mutex_init ( &mutex, NULL );

	/* 条件变量的初始化 */
	pthread_cond_init ( &cond, NULL );

	/* 创建线程 */
	pthread_create ( &tid1, NULL, ( void * )thread1, NULL );
	pthread_create ( &tid2, NULL, ( void * )thread2, NULL );

	do {
		/* 解除特定线程阻塞<顺序激活> */
		pthread_cond_signal ( &cond );
	} while ( 1 );

	sleep ( 6 );
	pthread_exit ( 0 );

	return EXIT_SUCCESS;
}

