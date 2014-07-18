/*
 * =====================================================================================
 *
 *       Filename:  my_unlink.c
 *
 *    Description:  实现unlink的应用，删除文件temp
 *
 *        Version:  1.0
 *        Created:  2014年07月16日 19时53分54秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sweetheart502, liyingxiao502@gmail.com
 *        Company:  Class 1304 of Software Engineering
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

void my_err (char *err_string, int line)
{
	fprintf (stderr, "line: %d\n", line);
	perror (err_string);
	exit (0);
}

int main(int argc, char *argv[])
{
	int fd;
	char buf[32];

	//创建一个文件temp
	if ((fd = open ("temp", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU)) == -1) {
		my_err ("open", errno);
	}

	//调用unlink函数
	if (unlink ("temp") == -1) {
		my_err ("unlink", errno);
	}
	printf ("file unlinked\n");

	//在unlink之后对文件进行读写操作.
	if (write (fd, "temp", 5) == -1) {
		my_err ("write", errno);
	}
	
	if (lseek (fd, 0, SEEK_SET) == -1) {
		my_err ("lseek", errno);
	}

	if (read (fd, buf, 5) == -1) {
		my_err ("read", errno);
	}
	printf ("%s\n", buf);
	/*程序在unlink之后如果出现崩溃，则在程序结束时，temp也不会遗留下来*/

	return EXIT_SUCCESS;
}

