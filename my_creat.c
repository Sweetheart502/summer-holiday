/*
 * =====================================================================================
 *
 *       Filename:  my_creat.c
 *
 *    Description:  利用open或creat系统调用来创建一个新文件。
 *
 *        Version:  1.0
 *        Created:  2014年07月14日 11时02分11秒
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

int main(int argc, char *argv[])
{
	int fd;

	if ((fd = open ("example_creat", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR)) == -1) {
	//if ((fd = creat("example_62.c", S_IRWXU)) == -1) {
		perror ("open");
		//printf ("open: %s 	with error: %d\n", strerror(errno), errno);
		exit (1);
	} 
	else {
		printf ("creat the file success.\n");
	}

	close (fd);

	return EXIT_SUCCESS;
}

