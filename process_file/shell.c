/*
 * =====================================================================================
 *
 *       Filename:  shell.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年07月25日 16时29分13秒
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
#include <sys/wait.h>

#define N 80

//char * p;

void setup ( char *buf, char * argv[] )
{
	int i = 0;
	char *p;

	for ( p = buf; ; p++ ) {
		argv[i++] = p;

		while ( *p != ' ' && *p != '\0' ) {
			p++;
		}

		if ( *p == '\0' ) /* 字符串结束 */
		      break;

		*p = '\0';
	}

	argv[i] = NULL; 	/* argv末尾是以NULL结束的 */

	execvp ( argv[0], argv );
}

int main( )
{
	char buf[N];
	char * argv[N];

	while ( 1 ) {
		printf ( "sweetheart@xiao $ " );
		gets ( buf );
		setup ( buf, argv );
	}

	return EXIT_SUCCESS;
}

