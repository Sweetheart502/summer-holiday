/*
 * =====================================================================================
 *
 *       Filename:  book_strcpy.c
 *
 *    Description:  the strcpy function of book -- to check input.
 *
 *        Version:  1.0
 *        Created:  2014年08月04日 22时04分49秒
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

char * book_strcpy ( char * strDest, const char * strSrc )
{
	char *p_return = strDest;
	
	/* check the point is NULL */
	if ( strDest == NULL || strSrc == NULL ) {
		fprintf ( stderr, "NULL POINT" );
		return NULL;
	}

	while ( ( * strDest++ = * strSrc++ ) != '\0' );

	return p_return;
}

int main(int argc, char *argv[])
{
	char 	string1[32];
	char 	string2[32];
	int 	c;
	int 	i = 0;

	printf ( "please input your string:\n" );

	//check the length of the string
	while ( ( ( c = getchar () ) != '\n' ) && ( c != EOF ) && ( i < 31 ) ) {
		string2[i] = c;
		i++;
	}
	string2[i] = '\0';

	//check the returned value
	if ( book_strcpy ( string1, string2 ) == NULL ) {
		fprintf ( stderr, "return from book_strcpy" );
		exit ( 1 );
	}

	printf ( "string1: %s\n", string1 );
	printf ( "string2: %s\n", string2 );

	return EXIT_SUCCESS;
}

