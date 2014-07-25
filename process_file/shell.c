/*
 * =====================================================================================
 *
 *       Filename:  shell.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年07月25日 16时29分13秒
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
#include <sys/wait.h>

#define N 80

void setup ( char *buf, char * argv[] )
{
	int 	i = 0; 					
	char 	*p;

	for ( p = buf; ; p++ ) {
		argv[i++] = p;

		while ( *p != ' ' && *p != '\0' ) { 	/* 没有出现空格或者没有到达字符串末尾，都写入同一个字符串中，作为参数或命令 */
			p++;
		}

		if ( *p == '\0' ) 			/* 字符串结束 */
		      break;

		*p = '\0'; 				/* 使每一个字符串都以'\0'结尾 */
	}

	argv[i] = NULL; 				/* argv末尾是以NULL结束的 */

	execvp ( argv[0], argv ); 			/* 调用函数执行可执行程序 */
}

int main( )
{
	char buf[N]; 					/* 存储输入的命令 */
	char * argv[N]; 				/* 获得命令及参数，即将输入内容进行分割 */

	while ( 1 ) {
		printf ( "sweetheart@xiao $ " ); 	/* 输出shell的提示符 */
		gets ( buf ); 				/* 输入命令字符串 */
		setup ( buf, argv ); 			/* 执行命令信息 */
	}

	return EXIT_SUCCESS;
}

