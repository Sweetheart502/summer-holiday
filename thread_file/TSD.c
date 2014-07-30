/*
 * =====================================================================================
 *
 *       Filename:  TSD.c
 *
 *    Description:  创建和使用线程的私有数据，学习相关的四个函数。
 *
 *        Version:  1.0
 *        Created:  2014年07月29日 20时43分33秒
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

/*
 * int pthread_key_create ( pthread_key_t * key, void ( * destructor )( void * ) );
 * int pthread_key_delete ( pthread_key_t key );
 * void * pthread_getspecific ( pthread_key_t key );
 * int pthread_setspecific ( pthread_key_t key, const void * pointer );
 */

pthread_key_t 	key; 				/* 定义全局变量 */

void * thread2 ( void * arg )
{
	int 		tsd = 1;

	/* 与key相关联 */
	pthread_setspecific ( key, ( void * ) tsd );

	printf ( "Thread_tid is %lu, the key is %d.\n", pthread_self (), ( int ) pthread_getspecific ( key ) );
}

void * thread1 ( void * arg )
{
	int 		tsd = 0;
	pthread_t 	tid2;

	/* 与key相关联 */
	pthread_setspecific ( key, ( void * ) tsd );

	/* 新创建一个线程 */
	pthread_create ( &tid2, NULL, ( void * ) thread2, NULL );

	sleep ( 2 );

	printf ( "Thread_tid is %lu, the key is %d.\n", pthread_self (), ( int ) pthread_getspecific ( key ) );
}

int main(int argc, char *argv[])
{
	pthread_t 	tid;

	printf ( "STARTING\n" );

	/* 创建私有数据key */
	pthread_key_create ( &key, NULL ); 		/* 第二个参数为NULL，调用默认的销毁函数 */

	/* 创建线程 */
	pthread_create ( &tid, NULL, ( void * ) thread1, NULL );

	sleep ( 3 );

	/* 注销创建的key */
	pthread_key_delete ( key );

	printf ( "ENDING\n" );

	/* 按照书上的主函数sleep(3),而thread1中sleep(5),导致
	 * 只输出了thread2的语句，然后还没有输出thread1的语句，
	 * 主线程的时间就结束而终止，因此之前只打印一条语句。
	 * 因此我将主函数中的改成了sleep(8)。
	 */

	/* 经过好几次修改测试，我认为只要主函数时间大于调用函数，则就可以正常输出两条语句。 */

	return EXIT_SUCCESS;
}

