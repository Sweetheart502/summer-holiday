/*
 * =====================================================================================
 *
 *       Filename:  my_cd.c
 *
 *    Description:  利用chdir函数实现cd功能
 *
 *        Version:  1.0
 *        Created:  2014年07月16日 20时54分50秒
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
#include <errno.h>
#include <linux/limits.h>

void my_err (char *err_string, int line)
{
	fprintf (stderr, "line: %d\n", line);
	perror (err_string);
	exit (0);
}

int main(int argc, char *argv[])
{
	char buf[PATH_MAX + 1];

	//判断参数个数是否正确
	if (argc < 2) {
		//给出正确的命令行输入格式提示
		printf ("my_cd <target path>");
		exit (1);
	}

	//调用chdir函数，成功执行返回0，失败返回-1
	if (chdir (argv[1]) == -1) {
		my_err ("chdir", errno);
	}

	//调用getcwd函数，获取当前目录
	if (getcwd (buf, 512) < 0) {
		my_err ("getcwd", errno);
	}
	//输出当前目录
	printf ("%s\n", buf);

	/*从程序运行结果可以看出，在shell运行后并不能如cd命令一样进行目录的切换。
	 *这是因为chdir只影响调用该函数的进程，对其他进程，如其父进程的当前工作
	 *目录，则修改不了。这也是cd命令作为少数几个shell内置明亮的原因。
	 **/

	return EXIT_SUCCESS;
}

