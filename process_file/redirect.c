/*
 * =====================================================================================
 *
 *       Filename:  redirect.c
 *
 *    Description:  关于输出重定向的实例及区别。
 *
 *        Version:  1.0
 *        Created:  2014年07月27日 17时40分11秒
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

int main(int argc, char *argv[])
{
	system ( "ls > file1" );
	/* 
	 * 表示将ls命令执行后结果送到指定文件file1中，
	 * 此时的文件存在则清除文件原内容，不存在则新建一个该文件。
	 */

	 system ( "ls >> file2" );
	 /*
	  * 将命令执行的显示结果附加到指定文件中。
	  * 此时文件存在则直接追加写入文件，不存在则新建一个该文件。
	  */

	system ( "ls >! file3" );
	/*
	 * 将命令执行显示的结果送至指定的文件中。
	 * 此时文件存在则覆盖，不存在则不能进行。
	 */

	return EXIT_SUCCESS;
}

