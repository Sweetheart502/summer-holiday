/*
 * =====================================================================================
 *
 *       Filename:  show_files.c
 *
 *    Description:  利用程序实现：显示指定目录下的文件。
 *
 *        Version:  1.0
 *        Created:  2014年07月17日 08时23分53秒
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
#include <dirent.h>

int my_readir (char *path) 
{
	DIR *dir;
	struct dirent *ptr; 					//存储文件信息

	//打开目录
	if ((dir = opendir (path)) == NULL) {
		perror ("opendir");
		return -1;
	}

	//读取目录下文件信息
	/*readir函数从dir指向的目录中读取目录项信息，返回一个struct dirent的指针*/
	while ((ptr = readdir (dir)) == NULL) {
		//输出文件名称
		printf ("filename: %s\n", ptr->d_name);
	}

	//关闭目录
	closedir (dir);

	return 0;
}

int main(int argc, char **argv)
{
	//判断输入的参数个数是否有错误
	if (argc < 2) {
		printf ("listfile <target path>\n");
		exit (0);
	}

	//调用自定义函数，实现读取目录下文件
	if (my_readir (argv[1]) == -1) {
		exit (0);
	}

	return EXIT_SUCCESS;
}

