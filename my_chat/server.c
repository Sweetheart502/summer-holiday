
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
#include <time.h>
#include <fcntl.h>
#include <pthread.h>

/// #define DEBUG 1

#define  PORT 	4507 		/// the server's port
#define  LIST 	12 		/// the line's length

#define MAX_LEN 500 		/// the message's length

/// user_info
struct user {
	char username[32] ;
	char password[32] ;
	int  state ;
	int  socket ;
} ;

/// send msg
struct msg {
	char 		from[32] ; 			/// the sender's name
	char 		to[32] ; 			/// the receiver's name
	int 		command ;  			/// the client's command
	char 		content[MAX_LEN] ; 		/// the client's send_msg
	struct msg 	*next ; 			/// the next pointer
} ;

//the login's link
struct  line {
	char 		username[32] ; 			/// the enter's name
	int 		socket ; 			/// the enter's socket
	struct line	*next ; 			/// the next pointer
} ;


struct line *head ;

/// display the error and write it to file_error
void my_err ( char *string , int line ) ;

/// write the error to the file_error
void record_err ( char *string ) ;

/// write the error_func to the file_error
void record_wr ( int fd , char *string ) ;

/// write the error_time to the file_error
void record_time ( int fd ) ;

/// to accept the request
void process ( int sock_fd ) ;

/// to process it in the thread
void *thread_chat ( void *member ) ;

/// to register a user
void regist () ;

/// to log in the system
void login ( int conn_fd ) ;

/// void user_login (int conn_fd) ;

/// void link_server (  ) ;


/// write the error function's name
void record_wr ( int fd , char *string )
{
	if ( write ( fd , string , strlen (string) ) != strlen (string) ) {
		perror ( "write" ) ;
		exit (1) ;
	}
}

/// write the time to the file
void record_time ( int fd )
{
	time_t 	now ;
	
	time ( &now ) ;
	
	if ( write ( fd , ctime ( &now ) , strlen ( ctime (&now) ) ) != strlen ( ctime (&now) ) ) {
		perror ( "write" ) ;
		exit (1) ;
	}
}

/// display the error 
void my_err ( char *string , int line )
{
	fprintf ( stderr , "line: %d.\n" , line ) ;
	perror ( string ) ;
	
	/// record the error to the file of error
	record_err ( string ) ;
	
	/// make it exit
	exit (1) ;
}

/// record the error to the file of error
void record_err ( char *string )
{
	/// the file's discribtion
	int 	fd ;
	
	/// open the file 
	if ( ( fd = open ( "./error" , O_RDWR | O_CREAT | O_APPEND ) ) == -1 ) {
		perror ( "open" ) ;
		exit (1) ;
	}
	
	/// write the function name of the error
	record_wr ( fd , string ) ;
	
	
	/// write the "error"
	
	if ( write ( fd , " error at " , 10 ) != 10 ) {
		perror ( "write" ) ;
		exit (1) ;
	}
	
	/// write the time to the file
	record_time ( fd ) ;
	
	close ( fd ) ;
}


/// creat a empty link
struct line *creatlink ()
{
	struct line 	*head , *p ;
	
	head = p = ( struct line * ) malloc ( sizeof ( struct line ) ) ;
	
	p->next = NULL ;
	
	return head ;
}


/// delete the user that will quit
void delete_link ( char *string )
{
	struct line *p , *q ;
	
	p = head->next ;
	
	for ( q = head ; p ; q = p , p = p->next ) {
		/// 寻找即将下线用户并删除结点
		if ( strcmp (p->username , string ) == 0 ) {
			q->next = p->next ;
			free (p) ;
			break ;
		}
	}
}


/// list the user that log in successfully
void list_line ( char *string )
{
	struct line *p = head->next ;
	
	strcpy ( string , "\n在线用户: \n" ) ;
	
	
	while ( p ) {
		/// 将用户名信息写入
		strcat ( string , "\t " ) ;
		strcat ( string , p->username ) ;
		strcat ( string , " \n" ) ;
		
		
		/// 遍历在线链表
		p = p->next ;
	}
}

void record_chat ( char *string )
{
	int 	fd ;
	
	if ( ( fd = open ( "./chat" ,  O_RDWR | O_CREAT | O_APPEND , 436 ) ) == -1 ) {
		my_err ( "open" , __LINE__ ) ;	
	}
	
	record_wr ( fd , string ) ;
	
	/// write the time to the file
	record_time ( fd ) ;
	
}

/// to process it in the thread
void *thread_chat ( void * member )
{
	/// 存储用户昵称和用户在服务端的套接字
	struct user 	user = *(struct user *)member ; 
	
	/// 保存recv的返回值
	int flag = 1 ;
	
	/// 发送信息的结构体
	struct msg 	info , sen ;
	
	/// 接受信息者fd 发送信息者fd 
	int 		send_fd , recv_fd ;
	
	
	/// 写日志时将所有信息存储在string中
	char 		string[500] ;
	
	/// 将在线用户更新
	struct line 	*p = head->next ;
	
	while (1) {
		/// 将结构体清空
		memset ( &info , 0 , sizeof (struct msg) ) ;
		
		/// 接收客户端信息
		flag = recv ( user.socket , &info , sizeof (struct msg) , 0 ) ;
		if ( flag < 0 ) {
			my_err ( "recv" , __LINE__ ) ;
		}
		else if ( flag == 0 ) {
			/// 接受信息为空时表示，断开连接
			printf( "客户端断开连接！" ) ;

			/// 使p指上来
			p = head->next ;

			while ( p != NULL ) {
				/// 寻找发送消息者的套接字
				if ( p->socket == user.socket) {
					break ;
				}
				p = p->next ;
			}
			
			/// 将该用户在在线链表中删除
			delete_link ( p->username ) ;
			if ( close( user.socket ) < 0 )
				perror( "close" ) ;

			/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
			
			return ;
		}

		#ifdef DEBUG
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~recv~info~~~~\n");
		printf("from: %s\n",info.from);
		printf("to :%s\n",info.to);
		printf("connect:%s\n",info.content);
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		#endif
		//		assert (ret == sizeof (struct msg)) ;0.
		
		
		/// 在服务器端显示语句
		printf ( "%s to %s : %s.\n" , info.from , info.to , info.content ) ;
		
		/// 将聊天信息写入文件chat
		strcpy ( string , info.from ) ;
		strcat ( string , " to " ) ;
		strcat ( string , info.to ) ;
		strcat ( string , " : " ) ;
		strcat ( string , info.content ) ;
		strcat ( string , " \0" ) ;
		
		record_chat ( string ) ;
		
		/// 如果是server则判断是不是list or quit，是则调用相应的函数，不是给它发送输入错误．
		
		if ( strcmp ( info.to , "server" ) == 0 ) {
			/// 内容识别
			if ( strcmp ( info.content , "list" ) == 0 ) {
				
				/// 将sen传过去，获得在线用户的字符串
				list_line ( sen.content ) ;
			} else if ( strcmp ( info.content , "quit" ) == 0 ) {
				
				strcpy ( sen.content , "the user is deleted.\n\0" ) ;

				/// 将该用户从在线链表中删除
				delete_link ( info.from ) ;
				
				if ( close ( user.socket ) < 0 )
					perror( "close" ) ;
				/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
				return ;
			} else {
				
				/// 说明发送命令有误
				strcpy ( sen.content , "Your command does not exit.\n" ) ;
			}

// 			p = head->next;
// 			
// 			while ( p != NULL ) {
// 				/// 寻找发送消息者的套接字
// 				if ( strcmp (p->username, info.from) == 0 ) {
// 					recv_fd = p->socket;
// 					break;
// 				}
// 				p = p->next;
// 			}
			
			strcpy ( sen.to , info.from ) ;
			strcpy ( sen.from , "server" ) ;

			/// 发送信息给发送消息者
			#ifdef DEBUG
			printf("~~~~~~~~~~~~~~~~~~~~~~~~~~send~~~~\n");
			printf("from: %s\n",sen.from);
			printf("to :%s\n",sen.to);
			printf("connect:%s\n",sen.content);
			printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
			#endif
			
			if ( send ( user.socket , &sen , sizeof ( struct msg ) , 0 ) < 0 ) {
				my_err ( "send" , __LINE__ ) ;
			}
			
		}
		else if ( strcmp ( info.to , "list" ) == 0 ) {

			/// 显示在线用户昵称
			list_line ( sen.content ) ;
			
			strcpy ( sen.to , info.from ) ;
			strcpy ( sen.from , "server" ) ;

			/// 发送信息给发送消息者
			#ifdef DEBUG
			printf("~~~~~~~~~~~~~~~~~~~~~~~~~~send~~~~\n");
			printf("from: %s\n",sen.from);
			printf("to :%s\n",sen.to);
			printf("connect:%s\n",sen.content);
			printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
			#endif
			
			if ( send ( user.socket , &sen , sizeof ( struct msg ) , 0 ) < 0 ) {
				my_err ( "send" , __LINE__ ) ;
			}
		}
		else if ( strcmp ( info.to , "group" ) == 0 ) {

			/*~~~~~~~~~~~群聊~~~~~~~~~~~*/
			p = head->next ;
			memset ( &sen , 0 , sizeof (struct msg) ) ; 
			/// 将内容显示为[group] info.content
			strcpy ( sen.from , info.from ) ;
			strcpy ( sen.to , info.to ) ;
			strcpy ( sen.content , "[" ) ;
			strcat ( sen.content , sen.to) ;
			strcat ( sen.content , "] " ) ;
			strcat ( sen.content , info.content ) ;

			while ( p != NULL ) {

				#ifdef DEBUG
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~send~~~~\n");
				printf("from: %s\n",sen.from);
				printf("to :%s\n",sen.to);
				printf("connect:%s\n",sen.content);
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				#endif
				
				if ( send ( p->socket, &sen, sizeof ( struct msg ), 0 ) < 0 ) {
					my_err ( "send" , __LINE__ ) ;
				}
				p = p->next ;
			}
		}
		else{
			
			p = head->next ;

			/// 寻找显现用户中是否存在info.to
			while ( p != NULL ) {
				if ( strcmp ( p->username , info.to ) == 0 ) {
					/// 寻找接收消息者的套接字
					send_fd = p->socket ;
					break ;
				}
				p = p->next ;
			}
			
			if ( p == NULL ) {
				strcpy ( sen.content , "您输入的用户不在线或者不存在．\n" ) ;
				
				record_chat ( sen.content ) ;
		
				strcpy ( sen.to , info.from ) ;
				strcpy ( sen.from , "server" ) ;

				#ifdef DEBUG
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~send~~~~\n");
				printf("from: %s\n",info.from);
				printf("to :%s\n",info.to);
				printf("connect:%s\n",info.content);
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				#endif
				
				if ( send ( user.socket , &sen , sizeof ( struct msg ) , 0 ) < 0 ) {
					my_err ( "send" , __LINE__ ) ;
				}
				
			} 
			else {
				/// 发送消息给接收消息者
				#ifdef DEBUG
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~send~~~~\n");
				printf("from: %s\n",info.from);
				printf("to :%s\n",info.to);
				printf("connect:%s\n",info.content);
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				#endif
				if ( send ( send_fd , &info , sizeof ( struct msg ) , 0 ) < 0 ) {
					my_err ( "send" , __LINE__ ) ;
				}
			}
			
		}
	}
	
}



/// list the line_user's link
struct line 		*head;

/// 主函数 
int main (int argc , char *argv[])
{
 	int 			sock_fd ;
	int 			optval ;

	struct sockaddr_in 	cli_addr , serv_addr ;


	/// 创建一个TCP套接字 
	printf ( "Now the system is creating the scket. \n" ) ;
	sock_fd = socket ( AF_INET , SOCK_STREAM , 0 ) ;
	if ( sock_fd == -1 ) { 	
		// failed 
		my_err ( "socket" , __LINE__ ) ;
	}


	/// 设置套接字，使其可以重新绑定端口 
	optval = 1 ;
	if ( setsockopt ( sock_fd , SOL_SOCKET , SO_REUSEADDR , ( void * ) &optval , sizeof ( int ) ) == -1 ) {
		/// 成功返回0， 失败返回-1
		my_err ( "setsockopt" , __LINE__ ) ;
	} 

	/// 初始化服务器端地址结构 
	memset ( &serv_addr , 0 , sizeof ( struct sockaddr_in ) ) ;
	/// type of the address
	serv_addr.sin_family = AF_INET ;
	/// the server's port
	serv_addr.sin_port = htons ( PORT ) ; 
	/// the IP address
	serv_addr.sin_addr.s_addr = htonl ( INADDR_ANY ) ; 


	/// bind the socket to the local's port
	printf ( "Now the system is binding the socket.  \n" ) ;
	if ( bind ( sock_fd , ( struct sockaddr * ) &serv_addr , sizeof ( struct sockaddr_in ) ) == -1 ) {
		/// 成功返回0，失败返回-1 
		my_err ( "bind" , __LINE__ ) ;
	}


	/// 将套接字转化为监听套接字 
	printf ( "Now the system is listening the socket.  \n" ) ;
	if ( listen ( sock_fd, LIST ) == -1 ) {
		my_err ( "listen" , __LINE__ ) ;
	}

	/// creat the line_link
	head = creatlink () ;

	getchar () ;

	system ( "clear" ) ;
	
	printf ( "\t\t\t   ==~==TCP SERVER IS OK==~==\n" ) ;
	
	/// enter the chat between the server and client
	while ( 1 ) {
		process ( sock_fd ) ;
	}

}


// process the communication of the server and the client 
void process ( int sock_fd )
{
	int 			cli_len ; 	/// 客户端地址长度
	pthread_t 		tid ; 		/// 线程ID
	struct sockaddr_in 	cli_addr ; 	/// 客户端地址
	
	int 			socket , ret ;
	
	struct user 		member ;
	struct line 		*p = head , *q ;
	
	/// to accept the client's request
	cli_len = sizeof ( struct sockaddr_in ) ;
	
	if ( (member.socket = accept ( sock_fd , ( struct sockaddr * )&cli_addr , &cli_len ) ) == -1 )	 {
		my_err ( "accept" , __LINE__ ) ;
	}
	
	/// display the ip
	printf ( " \n\t\t  -.- A new client %s is connected. -.-\n" , inet_ntoa (cli_addr.sin_addr) ) ;
	
	/// apply a node
	q = ( struct line * ) malloc ( sizeof ( struct line ) ) ;
	
	/// receive the client's msg
	memset ( member.username , 0 , 32 ) ;

	if ( ( ret = recv ( member.socket, member.username, 32, 0 ) ) < 0 ) {
		my_err ( "recv" , __LINE__ ) ;
	}
	
	assert ( ret == 32 ) ;
	
	strcpy ( q->username , member.username ) ;
	
	q->socket = member.socket ;
	
	/// display the login in the server
	printf ( "\n\t\t---------------------------------------------------" ) ;
	printf ( "\n\t\t\t >_<  username：%s\t socket: %d" , member.username , member.socket ) ;
	printf ( "\n\t\t---------------------------------------------------\n" ) ;
	
	
	/// insert the login to the tail
	while ( p->next != NULL ) {
		p = p->next ;
	}
	
	q->next = p->next ; 	
	/// 挂尾链
	p = p->next = q ;
	/// 挂前链，挪尾指针
	
	/// create a thread to process the command
	pthread_create ( &tid , NULL , (void *)thread_chat , (void*)&member ) ;
}

