#include "head.h"
#include "my_err.c"
#include "creatlink.c"
#include "process.c"

//list the line_user's link
struct line 		*head;

// 主函数 
int main(int argc, char *argv[])
{
 	int 			sock_fd;
	int 			optval;

	struct sockaddr_in 	cli_addr, serv_addr;


	/// 创建一个TCP套接字 
	printf ( "Now the system is creating the scket. \n" );
	sock_fd = socket ( AF_INET, SOCK_STREAM, 0 );
	if ( sock_fd == -1 ) { 	
		// failed 
		my_err ( "socket", __LINE__ );
	}


	/// 设置套接字，使其可以重新绑定端口 
	optval = 1;
	if ( setsockopt ( sock_fd, SOL_SOCKET, SO_REUSEADDR, ( void * ) &optval, sizeof ( int ) ) == -1 ) {
		// 成功返回0， 失败返回-1
		my_err ( "setsockopt", __LINE__ );
	} 

	/// 初始化服务器端地址结构 
	memset ( &serv_addr, 0, sizeof ( struct sockaddr_in ) );
	/// type of the address
	serv_addr.sin_family = AF_INET;
	/// the server's port
	serv_addr.sin_port = htons ( PORT ); 
	/// the IP address
	serv_addr.sin_addr.s_addr = htonl ( INADDR_ANY ); 


	/// bind the socket to the local's port
	printf ( "Now the system is binding the socket.  \n" );
	if ( bind ( sock_fd, ( struct sockaddr * ) &serv_addr, sizeof ( struct sockaddr_in ) ) == -1 ) {
		/// 成功返回0，失败返回-1 
		my_err ( "bind", __LINE__ );
	}


	/// 将套接字转化为监听套接字 
	printf ( "Now the system is listening the socket.  \n" );
	if ( listen ( sock_fd, LIST ) == -1 ) {
		my_err ( "listen", __LINE__ );
	}

	/// creat the line_link
	head = creatlink ();

	getchar ();

	system ( "clear" );
	
	printf ( "\t\t\t   ==~==TCP SERVER IS OK==~==\n" );
	
	/// enter the chat between the server and client
	while ( 1 ) {
		process ( sock_fd );
	}

}
