/*
 * =====================================================================================
 *
 *       Filename:  my_move.c
 *
 *    Description:  实现文件的移动。
 *
 *        Version:  1.0
 *        Created:  2014年07月16日 19时07分22秒
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
	/*检查参数个数的合法性*/
	if (argc < 3) {
		printf ("the input_style: my_move <old file> <new file>\n");
		exit (0);
	}

	/*rename会将参数oldpath所指定的文件名称改为参数newpath所指定的文件名称。若newpath所指定的文件已存在，则原文件会被删除。*/
	/*成功执行返回0，失败返回-1。*/
	if (rename (argv[1], argv[2]) == -1) {
		perror ("rename");
		exit (0);
	}

	//利用rename系统调用，把命令行第二个参数所制定的文件名更改为命令行第三个参数制定的文件名

	return EXIT_SUCCESS;
}
