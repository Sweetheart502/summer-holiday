/*
 * =====================================================================================
 *
 *       Filename:  show_inet.c
 *
 *    Description:  观察这几个函数的作用。
 *
 *        Version:  1.0
 *        Created:  2014年08月01日 17时06分22秒
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char *argv[])
{
	char 		buf[32];
	int 		ret = 0;
	int 		host = 0;
	int 		network = 0;
	unsigned int 	address = 0;
	struct in_addr 	in;

	in.s_addr = 0;

	/* 输入一个以.分隔的字符形式的IP地址 */
	printf ( "please input your ip address.\n" );
	fgets ( buf, 31, stdin );
	buf[31] = '\0';

	/* 示例使用inet_aton函数 */
	if ( ( ret = inet_aton ( buf, &in ) ) == 0 ) { 		/* 将字符串的IP地址转换成二进制的网络字节顺序的IP地址 */
		printf ( "inet_aton:\tinvalid address\n" );
	} else {
		printf ( "inet_aton: \t 0x%x\n", in.s_addr );
	}

	/* 示例使用inet_addr函数 */
	if ( ( address = inet_addr ( buf ) ) == INADDR_NONE ) { /* 将字符串的网络地址转换成网络字节顺序的二进制IP地址 */
		printf ( "inet_addr: \t invalid address\n" );
	} else {
		printf ( "inet_addr: \t 0x%x\n", address );
	}

	/* 示例使用inet_network函数 */
	if ( ( address = inet_network ( buf ) ) == -1 ) { 	/* 将字符串形式的网络地址转换成网络字节顺序的二进制IP地址 */
		printf ( "inet_network: \t invalid address\n" );
	} else {
		printf ( "inet_network: \t 0x%x\n", address );
	}

	/* 示例使用inet_ntoa函数 */
	if ( inet_ntoa ( in ) == NULL ) { 			/* 将值为in的网络字节顺序的二进制IP地址转换成以.分隔的字符串形式 */
		printf ( "inet_ntoa: \t invalid address\n" );
	} else {
		printf ( "inet_ntoa: \t %s\n", inet_ntoa ( in ) );
	}

	/* 示例使用inet_lnaof()与inet_netof()函数 */
	host = inet_lnaof ( in ); 				/* 从in中提取出主机地址，成功返回主机地址。 */
	network = inet_netof ( in ); 				/* 从in中提取出网络地址，成功返回主机字节顺序的网络地址 */
	printf ( "inet_lnaof: \t 0x%x\n", host );
	printf ( "inet_netof: \t 0x%x\n", network );

	in = inet_makeaddr ( network, host ); 			/* 将网络号和主机号的两个地址组合成一个网络地址 */
	printf ( "inet_makeaddr: 0x%x\n", in.s_addr );

	return EXIT_SUCCESS;
}

