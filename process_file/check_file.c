/*
 * =====================================================================================
 *
 *       Filename:  check_power.c
 *
 *    Description:  检查进程是否有权访问某文件。
 *
 *        Version:  1.0
 *        Created:  2014年07月21日 16时13分58秒
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
#include <fcntl.h>
//#include <errno.h>

void error ( char * err_string, int line )
{
	fprintf ( stderr, "line: %d\n", line );
	perror ( err_string );
	exit ( 1 );
}

int main(int argc, char *argv[])
{
	int fd;

	printf ( "The process's uid is %d, it's euid is %d.\n", getuid (), geteuid () );

	fd = open ( "test.c", O_RDWR );
	if ( fd == -1 ) {
		error ( "open", __LINE__ );
	}
	
	printf ( "Open successful!\n" );

	close (fd);

	return EXIT_SUCCESS;
}

