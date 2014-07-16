/*
 * =====================================================================================
 *
 *       Filename:  get_mode.c
 *
 *    Description:  实现获取文件属性
 *
 *        Version:  1.0
 *        Created:  2014年07月15日 16时58分45秒
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
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	struct stat buf;

	//检查参数个数
	if (argc != 2) {
		printf ("Usage: my_stat <filename>\n");
		exit (0);
	}

	//获取文件属性
	if (stat (argv[1], &buf) == -1)	{
		perror ("stat");
		exit (0);
	}

	//打印出文件属性
	printf ("device is: %llu\n", buf.st_dev);

	printf ("inode is: %lu\n", buf.st_ino);

	printf ("mode is: %o\n", buf.st_mode);

	printf ("number of hard links is: %d\n", buf.st_nlink);

	printf ("user ID of owner is: %d\n", buf.st_uid);

	printf ("group ID of owner is: %d\n", buf.st_gid);

	printf ("device type (if inode device) is: %llu\n", buf.st_rdev);

	
	printf ("total_size, in bytes is: %ld\n", buf.st_size);

	printf ("blocksize for filesystem I/O is: %ld\n", buf.st_blksize);

	printf ("number of blocks allocated is: %ld\n", buf.st_blocks);


	printf ("time of last access is: %s\n", ctime (&buf.st_atime));

	printf ("time of last modification is: %s\n", ctime (&buf.st_mtime));

	printf ("time of last change is: %s\n", ctime (&buf.st_ctime));

	return EXIT_SUCCESS;
}

