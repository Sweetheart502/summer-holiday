/*
 * =====================================================================================
 *
 *       Filename:  look_PATH.c
 *
 *    Description:  显示环境变量->env命令的实现
 *
 *        Version:  1.0
 *        Created:  2014年07月21日 16时47分58秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sweetheart502, liyingxiao502@gmail.com
 *        Company:  Class 1304 of Software Engineering
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

extern char ** environ;

int main(int argc, char *argv[])
{
	int i; 				//循环变量

	for ( i = 0; environ[i] != NULL; i++ ) { 
		/* 每行输出一个环境变量的信息 */
		printf ( "%s\n", environ[i] );
	}

	return EXIT_SUCCESS;
}

