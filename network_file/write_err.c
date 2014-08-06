/*
 * =====================================================================================
 *
 *       Filename:  write_err.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年08月06日 20时06分58秒
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
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int 	fd;
	time_t now;
	
	time ( &now );

	if ( ( fd = open ( "./error", O_RDWR | O_CREAT ) ) == -1 ) {
		perror ( "open" );
		exit ( 1 );
	}

	if ( write ( fd, ctime ( &now ), strlen ( ctime ( &now ) ) ) != strlen ( ctime ( &now ) ) ) {
		perror ( "write" );
		exit ( 0 );
	}

	return EXIT_SUCCESS;
}

