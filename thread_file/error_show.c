/*
 * =====================================================================================
 *
 *       Filename:  error_show.c
 *
 *    Description:  输出错误信息。
 *
 *        Version:  1.0
 *        Created:  2014年07月31日 17时28分13秒
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
#include <errno.h>

FILE * open_file ( char * filename )
{
	FILE * fp;
	//errno = 0;

	fp = fopen ( filename, "rt" );

	if ( fp == NULL ) {
		//perror ( "fopen" );
		/* perror打印错误信息到stderr即屏幕或命令行终端 */
		
		printf ( "errno is %d.\n", errno ); //失败时的errno

		printf ( "error:%s -> %s\n", filename, strerror ( errno ) );
		/* strerror函数根据参数errnum提供的错误码获取一个描述错误信息的字符串 */
		exit ( 1 );
	} else {
		printf ( "errno is %d.\n", errno ); //成功时的errno
		return fp;
	}
}

/* 主函数 */
int main(int argc, char *argv[])
{
	char * filename = "test";

	open_file ( filename );

	return EXIT_SUCCESS;
}

