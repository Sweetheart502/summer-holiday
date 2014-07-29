/*
 * =====================================================================================
 *
 *       Filename:  once_run.c
 *
 *    Description:  学习pthread_once ()函数。
 *
 *        Version:  1.0
 *        Created:  2014年07月28日 16时13分40秒
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

/* once初始化 */
pthread_once_t 	once = 0;
/*
 * 在Linux Threads中，实际“一次性函数”的执行状态有三种：
 * NEVER 	0 ; IN_PROGRESS 	1 ; DONE 	2.
 */

void run ( void )
{
	printf ( "Once_function is running.\n" );
}

/* 新建线程1 */
void * thread1 ( void * agr )
{
	pthread_t 	newtid1;

	/* 获取当前新建的线程ID */
	newtid1 = pthread_self ();
	/* 输出线程ID的结果 */
	printf ( "The newtid1 is %lu.\n", newtid1 );
	
	/* 验证一次执行的函数 */
	pthread_once ( &once, run );

	printf ( "Thread1 is over!\n" );
}

/* 新建线程2 */
void * thread2 ( void * agr )
{
	pthread_t 	newtid2;

	/* 获取当前新建的线程ID */
	newtid2 = pthread_self ();
	/* 输出线程ID的结果 */
	printf ( "The newtid2 is %lu.\n", newtid2 );
	
	/* 验证一次执行的函数 */
	pthread_once ( &once, run );

	printf ( "Thread2 is over!\n" );
}



int main(int argc, char *argv[])
{
	pthread_t 	tid1;
	pthread_t 	tid2;

	/* 创建线程 */
	pthread_create ( &tid1, NULL, thread1, NULL );
	pthread_create ( &tid2, NULL, thread2, NULL );

	sleep ( 3 );

	printf ( "main thread is exited.\n" );

	/* 从run函数执行一次就体现处pthread_once函数确实仅执行了一次 */

	return EXIT_SUCCESS;
}

