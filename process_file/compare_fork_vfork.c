/*
 * =====================================================================================
 *
 *       Filename:  compare_fork_vfork.c
 *
 *    Description:  比较fork函数和vfork函数的区别。观察父子进程的执行顺序和对父进程变量的修改说明两者的区别。
 *
 *        Version:  1.0
 *        Created:  2014年07月21日 14时29分29秒
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

/* 定义一个全局变量 */
int 	global_var = 1;

int main(int argc, char *argv[])
{
	pid_t 	pid;
	/* 定义一个局部变量 */
	int 	local_var = 2;
	int 	i; //循环变量

	/* 调用函数创建子进程 */
	//pid = fork ();
	pid = vfork ();
	
	if ( pid == -1 ) {
		error ( "fork", __LINE__ );
	//	error ( "vfork", __LINE__ );
	}
	else if ( !pid ) {
		for ( i = 0; i < 3; i++ ) {
			global_var++;
			local_var++;
			sleep ( 1 );
		}
		printf ( "I am child process! global_var = %d, local_var = %d.\n", global_var, local_var );
	}
	else {
		for ( i = 0; i < 3; i++ ) {
			global_var++;
			local_var++;
			sleep ( 1 );
		}
		printf ( "I am parent process! global_var = %d, local_var = %d.\n", global_var, local_var );
		exit ( 1 );
	}

	/*
	 * 根据运行结果得出以下结论：
	 * 	fork一个进程时，哪个进程先运行取决于系统的调度算法；
	 * 	vfork一个进程时，vfork保证子进程先运行，当它调用exec或exit之后，父进程才可能被调度运行。
	 * 	fork一个进程时，子进程只是完全复制父进程的资源；
	 * 	vfork一个进程时，子进程共享父进程的地址空间(子进程对该地址空间的任何数据修改同样为父进程所见)。
	 */
	exit ( 0 );
}

