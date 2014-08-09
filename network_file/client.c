/*
 * =====================================================================================
 *
 *       Filename:  client.c
 *
 *    Description:  客户端
 *
 *        Version:  1.0
 *        Created:  2014年07月31日 14时39分25秒
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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>

#define INVALID_USERINFO	'n' 		/* 用户信息无效 */
#define VALID_USERINFO 		'y' 		/* 用户信息有效 */
#define BUFSIZE 		1024

void record_err ( char * string )
{
	char 	str[255];
	time_t 	now;
	int 	len, fd;

	if ( ( fd = open ( "./error", O_RDWR | O_CREAT | O_APPEND ) ) == -1 ) {
		perror ( "open" );
		exit ( 1 );
	}

	strcpy ( str, string );
	strcat ( str, " error " );

	len = strlen ( str );

	if ( write ( fd, str, len ) != len ) {
		perror ( "write" );
		exit ( 1 );
	}

	time ( &now );
	len = strlen ( ctime ( &now ) );

	if ( write ( fd, ctime ( &now ), len ) != len ) {
		perror ( "write" );
		exit ( 1 );
	}

	close ( fd );
}


void dis_err ( char *string, int line )
{
	perror ( string );
	fprintf ( stderr, "line: %d\n", line );

	record_err ( string );

	exit ( 1 );
}

int my_recv ( int sock_fd, char * data_buf, int len )
{
	static char 	recv_buf[BUFSIZE]; 	/* 自定义缓冲区 */
	static char 	*pread; 		/* 指向下一次读取数据的位置 */
	static int 	len_remain = 0; 	/* 自定义缓冲区剩余字节数 */
	int 		i;

	/* 如果自定义缓冲区中没有数据，则从套接字读取数据 */
	if ( len_remain <= 0 ) {
		if (  ( len_remain = recv ( sock_fd, recv_buf, sizeof ( recv_buf ), 0 ) ) < 0 ) {
			dis_err ( "recv", __LINE__ );
		} else if ( len_remain == 0 ) {
			return 0; 		/* 目的使计算机端的socket连接关闭 */
		}

		pread = recv_buf; 		/* 重新初始化pread指针 */
	}

	/* 从自定义缓冲区中读取一次数据 */
	for ( i = 0; *pread != '\n'; i++ ) {
		if ( i > len ) {
			return -1; 		/* 防止指针越界 */
		}

		data_buf[i] = *pread++;
		len_remain--;
	}

	/* 去除结束标志 */
	len_remain--;
	pread++;

	return i; 				/* 读取成功 */
}


//
int get_userinfo ( char * buf, int len )
{
	int 		i;
	int 		c;

	if ( buf == NULL ) {
		return -1;
	}

	i = 0;
	
	while ( ( ( c = getchar () ) != '\n' ) && ( c != EOF ) && ( i < len - 2 ) ){
		buf[i++] = c;
	}

	buf[i++] = '\n';
	buf[i] = '\0';

	return 0;
}

void input_userinfo ( int sock_fd, const char * string )
{
	char 		input_buf[32];
	char 		recv_buf[BUFSIZE];
	int 		flag_userinfo;

	/* 输入用户信息直到正确为止 */
	do {
		printf ( "%s:", string );

		if ( get_userinfo ( input_buf, 32 ) < 0 ) {
			printf ( "error return from get_userinfo\n" );
			exit ( 1 );
		}

		if ( send ( sock_fd, input_buf, strlen ( input_buf ), 0 ) < 0 ) {
			dis_err ( "send", __LINE__ );
		}

		/* 从连接套接字上读取一次数据 */
		if ( my_recv ( sock_fd, recv_buf, sizeof ( recv_buf ) ) < 0 ) {
			dis_err ( "my_recv", __LINE__ );
		}

		if ( recv_buf[0] == VALID_USERINFO ) {
			flag_userinfo = VALID_USERINFO;
		} else {
			printf ( "%s error, input again.\n", string );
			flag_userinfo = INVALID_USERINFO;
		}

	} while ( flag_userinfo == INVALID_USERINFO ); 
}




int main(int argc, char *argv[])
{
	int 			i;
	int 			ret; 		/* recv返回值 */
	int 			sock_fd; 	/* socket的返回值 */
	int 			serv_port; 	/* 服务器端口 */
	struct sockaddr_in 	serv_addr; 	/* 服务器地址 */
	char 			recv_buf[BUFSIZE], in_buf[BUFSIZE];

	/* 检查参数个数 */
	if ( argc != 5 ) {
		printf ( "Usage: [-p] [serv_port] [-a] [serv_address]\n" );
		exit ( 1 );
	}

	/* 初始化服务器端地址结构 */
	memset ( &serv_addr, 0, sizeof ( struct sockaddr_in ) );
	serv_addr.sin_family = AF_INET;

	/* 从命令行获取服务器端的端口和地址 */
	for ( i = 1; i < argc; i++ ) {
		if ( strcmp ( "-p", argv[i] ) == 0 ) {
			serv_port = atoi ( argv[i + 1] );  		/* 将字符串转换成整型数 */

			if ( serv_port < 0 || serv_port > 65535 ) {
				printf ( "The recv_port is unvalid.\n" );
				exit ( 1 );
			} else {
				serv_addr.sin_port = htons ( serv_port );/* 转换成网络字节顺序 */
			}
			continue;
		}

		if ( strcmp ( "-a", argv[i] ) == 0 ) {
			if ( inet_aton ( argv[i + 1], &serv_addr.sin_addr ) == 0 ) { /* 失败返回０，成功返回非０值 */
				/* inet_aton函数将命令行参数指向的字符串形式的ip地址转换成二进制的网络字节顺序的ip地址存在第二个参数中 */
				printf ( "invalid server ip address.\n" );
				exit ( 1 );
			}
			continue;
		}
	}

	/* 检测是否少输入了某项参数 */
	if ( serv_addr.sin_port == 0 || serv_addr.sin_addr.s_addr == 0 ) {
		printf ( "Usage: [-p] [serv_port] [-a] [serv_address]\n" );
		exit ( 1 );
	}

	/* 创建一个TCP套接字 */
	printf ( "Now the client is creating a socket ~\n" );
	sock_fd = socket ( AF_INET, SOCK_STREAM, 0 );

	if ( sock_fd == -1 ) {
		dis_err ( "socket", __LINE__ );
	}

	/* 向服务器发送连接请求 */
	printf ( "Now the client is conneted with the server ~\n" );
	if ( connect ( sock_fd, ( struct sockaddr * ) &serv_addr, sizeof ( struct sockaddr ) ) < 0 ) {
		dis_err ( "connect", __LINE__ );
	}

	/* 输入用户名和密码 */
	input_userinfo ( sock_fd, "username" );
	input_userinfo ( sock_fd, "password" );
	
	ret = my_recv ( sock_fd, recv_buf, sizeof ( recv_buf ) );

	/* 读取欢迎信息并打印出来 */
	if ( ret == -1 ) {
		printf ( "data is too long.\n" );
		exit ( 1 );
	}

	for ( i = 0; i < ret; i++ ) {
		printf ( "%c", recv_buf[i] );
	}
	printf ( "\n" );

	while ( 1 ) {
		//init the in_ buf
		memset ( in_buf, 0, sizeof ( in_buf ) );

		if ( get_userinfo ( in_buf, sizeof ( in_buf ) ) == -1 ) {
			dis_err ( "get_userinfo", __LINE__ );
		}

		if ( ( send ( sock_fd, in_buf, sizeof ( in_buf ), 0 ) ) == -1 ) {
			dis_err ( "send", __LINE__ );
		}

		ret = my_recv ( sock_fd, recv_buf, sizeof ( recv_buf ) );

		if ( ret == -1 ) {
			dis_err ( "my_recv", __LINE__ );
		}

		for ( i = 0; i < ret; i++ ) {
			printf ( "%c", in_buf[i] );
		}
		printf ( "\n" );
	}


	close ( sock_fd );

	return EXIT_SUCCESS;
}

