/*
 * =====================================================================================
 *
 *       Filename:  zombie_process.c
 *
 *    Description:  僵尸进程
 *
 *        Version:  1.0
 *        Created:  2014年07月23日 16时28分26秒
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

int main(int argc, char *argv[])
{
	pid_t 	pid;

	while ( 1 ) {
		/* creat a new process */
		pid = fork ();

		if ( pid == -1 ) {
			/* creat is failed! */
			perror ( "fork error" );
			exit ( -1 );
		}
		else if ( !pid ) {
			/* child process */
			printf ( "I am a child process. I will exit.\n" );
			exit ( 0 );
		}
		else {
			/* parent process */
			printf ( "write your command. Look the stat(time is 20s).\n" );
			sleep ( 20 );
		}
	}

	/*
	 * 运行程序时，让其在后台运行即需要用到“&”，该命令出现在命令行末表示直接将命令丢到后台中执行。
	 * 这样在父进程睡眠的20s内，输入命令：ps -f 来显示当前有效用户下的进程较详细的状态信息。
	 * 通过几次子进程的退出，可以看到子进程成为僵尸状态。
	 * 之后，利用命令kill使其父进程退出，则那些子进程就会被init进程所领养并进行处理。
	 * 最后在查看下进程状态观察现象。
	 */

	return EXIT_SUCCESS;
}

