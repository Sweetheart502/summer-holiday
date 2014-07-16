/*
 * =====================================================================================
 *
 *       Filename:  test_umask.c
 *
 *    Description:  对函数umask的具体应用。
 *
 *        Version:  1.0
 *        Created:  2014年07月16日 18时22分40秒
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

int main(int argc, char *argv[])
{
	umask (0); 				//不屏蔽任何权限

	if ( creat ( "example_umask1", S_IRWXU | S_IRWXG | S_IRWXO ) == -1 ) {
		perror ( "creat" );
		exit (0);
	}

	umask (S_IRWXO); 			//屏蔽其他用户的所有权限

	if ( creat ( "example_umask2", S_IRWXU | S_IRWXG | S_IRWXO ) == -1 ) {
		perror ( "creat" );
		exit (0);
	}

	return EXIT_SUCCESS;
}

