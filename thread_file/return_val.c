/*
 * =====================================================================================
 *
 *       Filename:  return_val.c
 *
 *    Description:  关于信号中断执行时的返回值。
 *
 *        Version:  1.0
 *        Created:  2014年07月31日 20时26分31秒
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

int main(int argc, char *argv[])
{
	sleep ( 7 );

	/*
	 * 当sleep正常执行完之后，返回值为0;
	 * 当Ctrl-c中断查看返回值为130;
	 * 当Ctrl-\中断查看返回值为131.
	 * reason:
	 * Ctrl-c发送SIGINT,SIGINT=2,终止状态=128+信号编号.
	 * 因此返回值为130.
	 */

	return EXIT_SUCCESS;
}

