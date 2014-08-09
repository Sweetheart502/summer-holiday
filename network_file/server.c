/*
 * =====================================================================================
 *
 *       Filename:  server.c
 *
 *    Description:  The programe of server.
 *
 *        Version:  1.0
 *        Created:  2014年07月30日 18时23分42秒
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
#include <errno.h>
#include <fcntl.h>

#define SERV_PORT 	4507 	/* 服务器端的端口 */
#define LISTENQ 	12 	/* 连接请求队列的最大长度 */

#define INVALID_USER 	'n' 	/* 用户信息无效 */
#define VALID_USER 	'y' 	/* 用户信息有效 */

#define USERNAME 	0 	/* 接收到的是用户名 */
#define PASSWORD 	1 	/* 接收到的是密码 */


struct userinfo { 
	/* 保存用户名和密码的结构体 */
	char 	username[32];
	char 	password[32];
	int 	conn_fd;
	int 	on_line;
};


/* 定义变量并初始化 */
struct userinfo 	users[] = { 
	{ "linux", "linux", 0, 0 },
	{ "xiao", "xiao", 0, 0 },
	{ "ying", "ying", 0, 0 },
	{ "duo", "duo", 0, 0 },
	{ " ", " ", 0, 0 } 
	/* 以只含一个空格的字符串作为数组的结束标志 */
};

void send_data ( int conn_fd, const char *string );

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


void record ( char * string )
{
	int 	fd, len;
	time_t 	now;


	if ( ( fd = open ( "./login", O_RDWR | O_CREAT | O_APPEND ) ) == -1 ) {
		dis_err ( "open", __LINE__ );
	}

	//write the name to the login
	len = strlen ( string );

	if ( write ( fd, string, len ) != len ) {
		dis_err ( "write", __LINE__ );
	}

	if ( write ( fd, " \0", 1 ) != 1 ) {
		dis_err ( "write", __LINE__ );
	}

	//write the time to the login
	time ( &now );
	len = strlen ( ctime ( &now ) );
	if ( write ( fd, ctime ( &now ), len  ) != len ) {
		dis_err ( "write", __LINE__ );
	}

	close ( fd );
}

//display the users of on_line members
void dis_user ( int conn_fd )
{
	int i;

	for ( i = 0; users[i].username[0] != ' '; i++ ) {
		if ( users[i].on_line == 1 ) {
			printf ( " %s\n ", users[i].username );
		//	send_data ( conn_fd, users[i].username );
		//	send ( conn_fd, users[i].username, strlen ( users[i].username ), 0 );
		}
	}
}

/* 查看用户信息是否正确 */
int find_name ( const char *name )
{
	int i;

	if ( name == NULL ) {
		printf ( "The find name, NULL pointer.\n" );
		return -2;
	}

	for ( i = 0; users[i].username[0] != ' '; i++ ) {
		if ( strcmp ( users[i].username, name ) == 0 ) { /* check the exist of the inputed name.  */
			return i;
		}
	}

	return -1;
}


/* 发送数据 */
void send_data ( int conn_fd, const char *string )
{
	int flag;

	flag = send ( conn_fd, string, strlen ( string ), 0 );

	if ( flag == -1 ) {
		dis_err ( "send", __LINE__ );
	}
}


/* 主函数 */
int main(int argc, char *argv[])
{
	int 			sock_fd /* 套接字 */, conn_fd /* accept()执行成功，返回新的套接字描述符，失败返回-1 */;
	int 			optval; /* 在setsockopt()中，表示待设置的套接字选项的值 */
	int 			flag_recv = USERNAME; /* 表示接收到的是用户名还是密码 */
	int 			ret;
	int 			name_num;
	pid_t 			pid;
	socklen_t 		cli_len;
	struct sockaddr_in 	cli_addr, serv_addr;
	char 			recv_buf[128];

	/* 创建一个TCP套接字 */
	printf ( "Now the system is creating the scket ~ \n" );
	sock_fd = socket ( AF_INET, SOCK_STREAM, 0 );
	if ( sock_fd == -1 ) { 	/* failed */
		dis_err ( "socket", __LINE__ );
	}


	/* 设置套接字，使其可以重新绑定端口 */
	optval = 1;
	if ( setsockopt ( sock_fd, SOL_SOCKET, SO_REUSEADDR, ( void * ) &optval, sizeof ( int ) ) == -1 ) {
		/* 成功返回0， 失败返回-1 */
		dis_err ( "setsockopt", __LINE__ );
	} 

	/* 初始化服务器端地址结构 */
	memset ( &serv_addr, 0, sizeof ( struct sockaddr_in ) );
	serv_addr.sin_family = AF_INET; 			/* type of the address */
	serv_addr.sin_port = htons ( SERV_PORT ); 		/* 端口号 */
	serv_addr.sin_addr.s_addr = htonl ( INADDR_ANY ); 	/* IP地址 */


	/* 将套接字绑定在本地端口 */
	printf ( "Now the system is binding the socket ~ \n" );
	if ( bind ( sock_fd, ( struct sockaddr * ) &serv_addr, sizeof ( struct sockaddr_in ) ) == -1 ) {
		/* 成功返回0，失败返回-1 */
		dis_err ( "bind", __LINE__ );
	}


	/* 将套接字转化为监听套接字 */
	printf ( "Now the system is listening the socket ~ \n" );
	if ( listen ( sock_fd, LISTENQ ) == -1 ) {
		dis_err ( "listen", __LINE__ );
	}

	cli_len = sizeof ( struct sockaddr_in ); 		/* the length of the client */

	while ( 1 ) {
		/* 通过accept()接受客户端的连接请求，并返回连接套接字用于收发数据 */
		printf ( "Now the system is accepting the client's socket ~ \n" );
		conn_fd = accept ( sock_fd, ( struct sockaddr * ) &cli_addr, &cli_len );

		if ( conn_fd == -1 ) {
			dis_err ( "accept", __LINE__ );
		}

		printf ( "accept a new client, ip: %s\n", inet_ntoa ( cli_addr.sin_addr ) );

		/* 创建一个子进程处理接收到的连接请求 */
		if ( ( pid = fork () ) == 0 ) {
			/* 当前进程为子进程 */
			while ( 1 ) {
				/* 函数recv用来在TCP套接字上接收数据 */
				ret = recv ( conn_fd, recv_buf, sizeof ( recv_buf ), 0 );
				/* 成功返回接收到的字节数，失败返回-1 */
				if ( ret == -1 ) {
					dis_err ( "recv", __LINE__ );
				}
				recv_buf[ret - 1] = '\0'; 	/* 用字符串结束标志替换掉字符串中的'\n' */

				if ( flag_recv == USERNAME ) { 	/* 接收到的是用户名 */
					/* 查看是否存在该用户名,存在返回用户信息下标，不存在返回-1，出错返回-2 */
					name_num = find_name ( recv_buf );

					switch ( name_num ) {
						case -1:
							send_data ( conn_fd, "n\n" );
							break;
						case -2:
							exit ( 1 );
							break;
						default:
							//不在线则正常登陆,在线则提示已经在线并强制退出
							if ( users[name_num].on_line == 0 ) {
								send_data ( conn_fd, "y\n" );
								flag_recv = PASSWORD;
								break;
							} else {
								printf ( "The user has been on_line.Please exit.\n" );
								exit ( 1 );
							}

					}
				} else if ( flag_recv == PASSWORD ) {
					/* 查看密码是否匹配 */
					if ( strcmp ( users[name_num].password, recv_buf ) == 0 ) {
						//说明用户密码正确有效
						send_data ( conn_fd, "y\n" );
						//显示欢迎登录信息
						send_data ( conn_fd, "Welcome login my tcp server\n" );

						//登陆成功，则服务器端显示上线语句
						printf ( "%s login\n", users[name_num].username );

						//将该用户套接字存储起来
						users[name_num].conn_fd = conn_fd;

						//将该用户on_line改为在线
						users[name_num].on_line = 1;

						//登陆日志
						record ( users[name_num].username );

						break; /* 跳出当前循环 */
					} else {
						send_data ( conn_fd, "n\n" );
					}
				}
			}

			while ( 1 ) {
				//init 
				memset ( &recv_buf, 0, sizeof ( recv_buf ) );

				ret = recv ( conn_fd, recv_buf, sizeof ( recv_buf ), 0 );

				if ( ret == -1 ) {
					dis_err ( "recv", __LINE__ );
				}

				recv_buf[ret - 1] = '\0';

				if ( strcmp ( recv_buf, "byebye~\n" ) == 0 ) {
					printf ( "%s exit\n", users[name_num].username );
					//close ( sock_fd );
					//close ( conn_fd );
					exit ( 0 );
				} else {
					//如果接收到的是ls则调用函数显示在线人数
					if ( strcmp ( recv_buf, "ls\n" ) == 0 ) {
						dis_user ( conn_fd );
					} else {
						//发送接受信息给聊天用户
						send ( conn_fd, recv_buf, strlen ( recv_buf ), 0 ); /* 问题：如何找到你所要发送的用户，即如何区别各个用户 */
					}
					//					send_data ( conn_fd, recv_buf );
				}
			}
			//close ( sock_fd );
			//close ( conn_fd );
			//exit ( 0 ); 			/* 结束子进程 */
		} else {
			/* 当前进程是父进程，父进程关闭刚刚接收到的连接请求，执行accept等待其他连接请求 */
			close ( conn_fd );
		}
	}

	return EXIT_SUCCESS;
}

