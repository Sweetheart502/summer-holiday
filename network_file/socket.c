/*
 * =====================================================================================
 *
 *       Filename:  socket.c
 *
 *    Description:  create a socket.
 *
 *        Version:  1.0
 *        Created:  2014年08月01日 16时08分15秒
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
#include <sys/socket.h>


int main(int argc, char *argv[])
{
	int 	sock_fd;

	sock_fd = socket ( AF_INET, SOCK_STREAM, 0 );

	if ( sock_fd == -1 ) {
		perror ( "socket" );
		exit ( 1 );
	}

	printf ( "%d\n", sock_fd );

	return EXIT_SUCCESS;
}

