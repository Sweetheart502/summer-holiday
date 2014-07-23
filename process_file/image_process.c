/*
 * =====================================================================================
 *
 *       Filename:  image_process.c
 *
 *    Description:  进程映像
 *
 *        Version:  1.0
 *        Created:  2014年07月21日 17时02分01秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sweetheart502, liyingxiao502@gmail.com
 *        Company:  Class 1304 of Software Engineering
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[], char ** environ)
{
	int i;

	printf ( "I am a process image!\n" );

	printf ( "My PID is %d, my parent PID is %d!\n", getpid (), getppid () );

	printf ( "the uid is %d, the gid is %d.\n", getuid (), getgid () );

	for ( i = 0; i < argc; i++ ) {
		printf ( "argv[%d]: %s.\n", i, argv[i] );
	}

	return EXIT_SUCCESS;
}

