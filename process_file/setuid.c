/*
 * =====================================================================================
 *
 *       Filename:  setuid.c
 *
 *    Description:  学习setuid()。
 *
 *        Version:  1.0
 *        Created:  2014年07月29日 09时19分19秒
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

/*
 * #include <sys/types.h>
 * #include <unist.h>
 * int setuid (uid_t uid);
 * 若进程具有root权限，则函数将实际用户ID/有效用户ID设置为参数uid.
 * 若进程不具有root权限，但uid等于实际用户ID，则setuid只将有效用户ID设置为uid,不改变实际用户ID.
 * 若以上两点都不满足，则调用函数失败，返回-1，并设置error值为EPERM.
 */

void show_ids ()
{
	printf ( "Real uid = %d.\n effective uid = %d.\n", getuid (), geteuid () );
}

int main(int argc, char *argv[])
{
	uid_t 	uid;

	/* 调用setuid函数前的用户ID情况 */
	show_ids ();

	/* 获得命令行所输入的uid */
	uid = atoi ( argv[1] );

	/* 调用函数改变uid */
	if ( setuid ( uid ) == -1) {
		perror ( "setuid" );
	//	exit ( 1 );
	}

	/* 调用函数成功后的用户ID情况 */
	show_ids ();

	return EXIT_SUCCESS;
}

