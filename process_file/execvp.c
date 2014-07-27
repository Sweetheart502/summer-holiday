/*
 * =====================================================================================
 *
 *       Filename:  execvp.c
 *
 *    Description:  用实践来了解execvp ()函数。
 *
 *        Version:  1.0
 *        Created:  2014年07月26日 10时00分28秒
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

int main( )
{
	/*
	 * #include <unistd.h>
	 * int execvp ( const char * file, char * const argc );
	 * 说明:execvp()会从PATH环境变量所指向的目录中查找符合参数file的文件，找到后执行该文件，然后将第二个参数argv传给该欲执的文件。
	 * 返回值：成功函数不返回，失败返回-1。
	 */

	char * argv[] = { "ls", "-la", "/home/sweetheart/lemon/holiday/", NULL };

	/* 调用函数测试其功能 */
	execvp ( argv[0], argv );

	return EXIT_SUCCESS;
}

