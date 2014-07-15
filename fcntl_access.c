/*
 * =====================================================================================
 *
 *       Filename:  fcntl_access.c
 *
 *    Description:  练习使用fcntl()函数
 *
 *        Version:  1.0
 *        Created:  2014年07月14日 17时00分48秒
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
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

void my_err (char *err_string, int line)
{
	fprintf (stderr, "line: %d", line);
	perror (err_string);
	exit (0);
}

int main(int argc, char *argv[])
{
	int ret;
	int access_mode;
	int fd;

	if ((fd = open ("example_access", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU)) == -1) {
		my_err ("open", errno);
	}

	/* 设置文件打开方式 */
	/* fcntl()失败返回-1 */
	if ((ret = fcntl (fd, F_SETFL, O_APPEND)) == -1) { 			//set设置
		my_err ("fcntl", errno);
	}

	printf ("%d\n", O_APPEND);

	printf ("first ret = %d\n", ret);

	/*获取文件打开方式*/
	//成功返回标志值，失败返回-1
	if ((ret = fcntl (fd, F_GETFL, 0)) == -1 ) { 			//get获取
		my_err ("fcntl", errno);
	}

	printf ("second ret = %d\n", ret);

	access_mode = ret & O_ACCMODE; 	
	/* O_ACCMODE是取得文件打开方式的掩码，实际上它的值是3，做与运算只是为了取得ret的最后两位的值 */

	printf ("access_mode = %d\n", access_mode);

	if (access_mode == O_RDONLY) {
		printf ("example_64 access mode: read only!\n");
	} else if (access_mode == O_WRONLY) {
		printf ("example_64 access mode: write only!\n");
	} else if (access_mode == O_RDWR) {
		printf ("example_64 access mode: read + write!\n");
	}

	/*打印某些数据，方便下面代码结果的分析*/
	printf ("ret = %d  O_APPEND = %d  O_NONBLOCK = %d  O_SYNC = %d \n", ret , O_APPEND, O_NONBLOCK, O_SYNC);

	if (ret & O_APPEND) {
		printf (" , append");
	}

	if (ret & O_NONBLOCK) {
		printf (" , nonblock");
	}

	if (ret & O_SYNC) {
		printf (" , sync");
	}

	printf ("\n");

	return EXIT_SUCCESS;
}

