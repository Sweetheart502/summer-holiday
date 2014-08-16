/*
 * =====================================================================================
 *
 *       Filename:  head.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年08月08日 18时38分45秒
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
	char 		to[32] ;  			/// the receiver's name
	int 		command ;  			/// the client's command
	char 		content[MAX_LEN] ; 		/// the client's send_msg
	struct msg 	*next ; 			/// the next pointer
} ;

/// the login's link
struct  line {
	char 		username[32] ;			/// the enter's name
	int 		socket ;			/// the enter's socket
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

/// the client's menu_function
void menu ( int conn_fd )
{
	/// 显示客户端刚开始一进来的选项：注册　登陆　退出
	char 	choice ;
		
	while (1) {
		/// 清屏
		system ( "clear" ) ;
		
		/// the client's menu
		printf ( "\t\t -----   WELCOME TO THE CLIENT  -------\n" ) ;
		printf ( "\t\t --------------------------------------\n" ) ;
		printf ( "\t\t 		A: SIGN UP\n\n" ) ;
		printf ( "\t\t 		B: LOG IN\n\n" ) ;
		printf ( "\t\t 		C: QUIT\n\n" ) ;
		printf ( "\t\t --------------------------------------\n" ) ;
		printf ( "\t\t ----------- ENJOY YOURSELF -----------\n" ) ;
		
		printf ( "\t\t please input your choice: " ) ;
		
		/// 输入选项
		scanf ( "%c" , &choice ) ;
		
		/// switch结构　客户做出选择
		switch ( choice ) {
			case 'A':
				/// 用户注册
				regist () ;
				/// system ( "cat info" );
				getchar () ;
				break ;
			case 'B':
				/// 用户登录
				login ( conn_fd ) ;
				break ;
			case 'C':
				/// 用户退出
				printf ( "\n\n\t\t You have successfully quit the chat.\n " ) ;
				exit (0) ;
			default :
				/// 出错循环
				printf ( "Your CHOICE does not exist, please enter again.\n" ) ;
				getchar () ;
		}
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

/// record the user's login
void record_login ( char * string )
{

}

/// record the error to the file of error
void record_err ( char *string )
{
	/// the file's discribtion
	int 	fd ;
	
	/// open the file 
	if ( ( fd = open ( "./error" , O_RDWR | O_CREAT | O_APPEND , 436 ) ) == -1 ) {
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


/// 注册用户并将其写入用户信息文件
void regist ()
{
	FILE 		*fp ;
	int 		i , flag ;
	struct user 	new , old[20] ;
	
	while (1) {
		
		system ( "clear" ) ;
		
		printf ( "\t\t ==>== WELCOME TO REGIST ==<==\n" ) ;
		printf ( "\t\t  input username & password\n" ) ;
		
		scanf ( "\t\t%s %s" , new.username , new.password ) ;
		getchar () ;
		
		/// open the file_info
		if ( ( fp = fopen ("./info" , "rt" ) ) == NULL ) {
			my_err ( "fopen" , __LINE__ ) ;
		}
		
		/// read the file_info and to check the new user
		i = 0 ;
		flag = 0 ;
		while ( fscanf ( fp , "%s %s %d %d\n" , old[i].username , old[i].password , &old[i].state , &old[i].socket ) != EOF ) {
			if ( !strcmp ( new.username , old[i].username )  ) {
				printf ( "\n\nthe name is existed.\n" ) ;
				
				/// the name have been existed
				flag = 1 ;
				getchar () ;
				break ;
			}
			i++ ;
		}
		fclose (fp) ;
		
		/// flag = 0->OK; flag = 1->NO
		if ( flag == 0 ) {
			/// init some data
			new.state = 0 ;
			new.socket = 0 ;
			
			/// open the file_info again
			if ( ( fp = fopen ("./info" , "at") ) == NULL ) {
				my_err ( "fopen" , __LINE__ ) ;
			}
			
			/// update the file_info
			fprintf ( fp , "%s %s %d %d\n" , new.username , new.password , new.state , new.socket ) ;
			
			fclose ( fp ) ;
			
			printf ( "\n\n\t\t\tCongratrulation!\n" ) ;
			break ;
		}
	}
	getchar () ;
}

/// change the exiter's state and socket
void exit_change ( char *name )
{
	FILE 		*fp ;
	int 		i , sum ;
	struct user	chater[20] ;
	
	/// first to read it to here
	if ( ( fp = fopen ( "./info" , "rt" ) ) == NULL ) {
		my_err ( "fopen" , __LINE__ ) ;
	}
	
	i = 0 ;
	while ( fscanf ( fp , "%s %s %d %d\n" , chater[i].username , chater[i].password , &chater[i].state , &chater[i].socket ) != EOF ) {
		if ( strcmp ( chater[i].username , name ) == 0 ) {
			chater[i].state = 0 ;
			chater[i].socket = 0 ;
		}
		i++ ;
	}
	sum = i ;
	fclose (fp) ;
	
	/// then to update it to file
	if ( ( fp = fopen ("./info" , "wt") ) == NULL ) {
		my_err ( "fopen" , __LINE__ ) ;
	}
	
	i = 0 ;
	while ( i < sum ) {
		fprintf ( fp , "%s %s %d %d\n" , chater[i].username , chater[i].password , chater[i].state , chater[i].socket ) ;
		i++ ;
	}
	
	fclose (fp) ;
}


/// 验证用户登录的身份是否合法
void identify (struct user *new , int conn_fd )
{	
	/// do statistics
	int 	count = 0 ;
	
	/// receive the username
	char 	send_msg[32] ; 	
	
	/// 用户身份验证，三次机会
	while ( count < 3 ) {
		if ( user_login ( conn_fd , new ) == 0 ) {
			/// 登陆成功
			printf ( "\n\n\t\tCongratrulation!\n" ) ;
			
			/// 暂停：使用户看到提示信息
			getchar () ;
			break ;
		}
		else {
			/// 登录失败
			count++ ;
		}
		
	}
	
	/// 表示登录失败
	if ( count == 3 ) {
		printf ( "your have made it three times , so the system command you to exit.\n" ) ;
		
		/// 暂停：使用户看到提示信息
		getchar () ;
		exit (1) ;
	} 
	
	/// send the success
	strcpy ( send_msg , new->username ) ;
	
	
	/// 将用户昵称发送给服务器端
	if ( send ( conn_fd , send_msg , sizeof (send_msg) , 0 ) < 0 ) {
		my_err ( "send" , __LINE__ ) ;
	}
	
}


void *recv_c ( void *arg )
{
	struct user *user = (struct user *)arg ;
	struct msg  	rec ;
	int 	    	ret ;
	int 		flag = 1 ;
	
	//	printf ("user name %s , socket %d \n" , user ->username , user -> socket) ;
	
	while ( 1 ) {
		memset ( &rec , 0 , sizeof (struct msg) ) ;

		flag = recv ( user->socket , &rec , sizeof (struct msg) , 0 );
		if ( flag < 0 ) {
			my_err ( "recv" , __LINE__ ) ;
		}
		else if ( flag == 0 )
		{
			printf( "与服务器连接断开！\n" ) ;
			exit (0) ;
		}

		#ifdef DEBUG
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~rec~~~~\n");
		printf("from: %s\n",rec.from);
		printf("to :%s\n",rec.to);
		printf("connect:%s\n",rec.content);
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		#endif
		
		printf ( "%s : %s\n" , rec.from , rec.content ) ;
	}
}

void *send_c ( void *arg ) 
{
	struct user 	*usr = (struct user *)arg ;
	struct msg 	sen ;
	char 		quit[10];
	
	
	while ( 1 ) {
		memset ( &sen , 0 , sizeof (struct msg) ) ;
		
		strcpy ( sen.from , usr->username) ;
		
		/// 输入您所要聊天的对象
		printf ( "请输入您所要聊天的对象，或者输入list查询在线情况，输入group进入群聊，输入quit退出 .\n " ) ;
		scanf ( "%s" , sen.to ) ;
		getchar () ;
		if ( strcmp ( sen.to , "list" ) == 0 ) {
			strcpy ( sen.content , "list" ) ;
			strcpy ( sen.to , "list" ) ;

			#ifdef DEBUG
			printf("~~~~~~~~~~~~~~~~~~~~~~~~~~send~~~~\n");
			printf("from: %s\n",sen.from);
			printf("to :%s\n",sen.to);
			printf("connect:%s\n",sen.content);
			printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
			#endif
			
			if ( send ( usr->socket , &sen , sizeof ( struct msg ) , 0 ) < 0 ) {
				my_err ( "sen" , __LINE__ ) ;
			}	
		} else if ( strcmp ( sen.to , "quit" ) == 0 ) {
			
			/// should send quit to server , and the serner delete the user
			strcpy ( sen.content , "quit" ) ;
			strcpy ( sen.to , "server" ) ;
			
			#ifdef DEBUG
			printf("~~~~~~~~~~~~~~~~~~~~~~~~~~send~~~~\n");
			printf("from: %s\n",sen.from);
			printf("to :%s\n",sen.to);
			printf("connect:%s\n",sen.content);
			printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
			#endif
			
			if ( send ( usr->socket , &sen , sizeof ( struct msg ) , 0 ) < 0 ) {
				my_err ( "sen" , __LINE__ ) ;
			}
			
			exit_change ( sen.from ) ;
			break ;
		} else {
			
			/// system ( "clear" );
			
			printf ( "\n\t\t -------------------------------------------\n" ) ;
			printf ( "\n\t\t           You will chat with %s .          \n" , sen.to ) ;
			printf ( "\n\t\t        WANT TO QUIT , ENTER QUIT .         \n" ) ;
			printf ( "\n\t\t -------------------------------------------\n" ) ;
			
			//*printf ( "%s: " , sen.from ) ;
			
			
			while ( 1 ) {
			
				/// 将发送者昵称显示
				usleep ( 10000 ) ;
				printf ( "%s: " , sen.from ) ;

				/// 输入发送消息内容
				/// scanf ( "%s" , sen.content ) ;
				gets ( sen.content );
				
		///		getchar () ;
			
				if ( strcmp ( sen.content , "quit" ) == 0 ) {
					break ;
				} else {
					
					/// 发送数据包
					#ifdef DEBUG
					printf("~~~~~~~~~~~~~~~~~~~~~~~~~~send~~~~\n");
					printf("from: %s\n",sen.from);
					printf("to :%s\n",sen.to);
					printf("connect:%s\n",sen.content);
					printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
					#endif

					if ( send ( usr->socket , &sen , sizeof (struct msg) , 0 ) < 0 ) {
						my_err ( "send" , __LINE__ ) ;
					}
				}
			}
			
			printf ( " 您已经退出与%s聊天!\n ", sen.to ) ;
			
		}
		
	}
		
}




void login ( int conn_fd )
{
	char 		name[32] ;
	pthread_t 	tid1 ;
	pthread_t 	tid2 ;
	struct user 	new ;
	
	identify (&new , conn_fd) ;
	
	int 	sockopt = 1 ;
	if ( setsockopt ( conn_fd , SOL_SOCKET , SO_KEEPALIVE , &sockopt , sizeof ( sockopt ) ) < 0 ) {
		perror ( "setsockopt" ) ;
	}
	
	pthread_create ( &tid2 , NULL , send_c , (void*)&new ) ;
	pthread_create ( &tid1 , NULL , recv_c , (void*)&new ) ;
	
	
	sleep (10000) ;
}


int link_server ()
{
	/// 用户选择登陆之后，先进行与服务器的连接
	int 			conn_fd ; 		/// the client's socket
	int 			serv_port ;
	char 			ip[32] ;
	struct sockaddr_in 	serv_addr ;
	int 			count = 0 ;
	
	/// init the serv_addr
	memset ( &serv_addr , 0 , sizeof (struct sockaddr_in) ) ;
	serv_addr.sin_family = AF_INET ;
	
	/// input the two arguments
	printf ( "please input the port and the ip:\n" ) ;
	scanf ( "%d %s" , &serv_port , ip ) ;
	
	/// check the argument's validity
	if ( serv_port < 0 || serv_port > 65535 ) {
		printf ( "invalid port.\n" ) ;
		exit (1) ;
	} else {
		serv_addr.sin_port = htons ( serv_port ) ;
	}
	
	
	if ( inet_aton ( ip, &serv_addr.sin_addr ) < 0 ) {
		printf ( "invalid ip.\n" ) ;
		exit (1) ;
	}
	
	#ifdef DEBUG
	printf ( "%s\n", ip );
	#endif
	
	/// creat the conn_fd
	printf ( "Now the client is creating the socket \n" ) ;
	conn_fd = socket ( AF_INET , SOCK_STREAM , 0 ) ;
	if ( conn_fd == -1 ) {
		my_err ( "socket" , __LINE__ ) ;
	}
	
	/// be connected with the server
	printf ( "Now the client is connected with the server ~\n" ) ;
	if ( connect ( conn_fd , ( struct sockaddr * )&serv_addr , sizeof ( struct sockaddr) ) == -1 ) {
		my_err ( "connect" , __LINE__ ) ;
	}
	
	return conn_fd ;
}

/// input the user_info and identity its validity
int user_login ( int conn_fd , struct user *new )
{
	/// 输入用户名和密码，并验证是否存在，存在且未登录则登陆，并修改其状态以及套接字数值．
	FILE 		*fp ;
	struct user	chater[20] ;
	char 		recv_msg[MAX_LEN] ;
	int 		flag = 0 ;
	int 		lemon = 0 ;
	int 		i , sum ;
	
	
	printf ("socket is %d \n" , conn_fd ) ;
	
	while (1){
		/// 进入身份验证页面
		system ( "clear" ) ;
		
		printf ( "\n\t\t---------------------------------------\n" ) ;
		printf ( "\t\t\t ------ WELCOME TO LOGIN ------\n" ) ;
		
		printf ( "\n\n\n\t\t\t   username: " ) ;
		printf ( "\n\n\t\t\t   password: \33[2A" ) ;
		
		/// 输入用户昵称
		scanf ( "%s" , new->username ) ;
		printf ( "\33[2B\t\t\t             \33[1A" ) ;
		
		/// 输入用户密码,使其不回显
		system ( "stty -echo" ) ;
		scanf ( "%s" , new->password ) ;
		system ( "stty echo" ) ;
		new->socket = conn_fd ;

		/// 清除缓存
		getchar () ;
		
		/// open the file_info
		if ( ( fp = fopen ( "./info" , "rt" ) ) == NULL ) {
			my_err ( "fopen" , __LINE__ ) ;
		}
		
		/// 设置循环变量并初始化
		i = 0 ;
		while ( !feof ( fp ) ) {
			/// 从文件中读出用户信息
			fscanf ( fp , "%s %s %d %d\n" , chater[i].username , chater[i].password , &chater[i].state , &chater[i].socket ) ;
			
			/// 比较用户信息是否正确
			if ( strcmp ( new->username , chater[i].username ) == 0 ) {
				/// 用户名匹配
				if ( strcmp ( new->password, chater[i].password ) == 0 ) {
					/// 用户密码匹配
					if ( chater[i].state == 0 ) {

						/// 在服务器端显示用户登陆成功
						printf ( "\n\n\t\t" ) ;
						printf ( "%s login" , chater[i].username ) ;

						record_login ( chater[i].username ) ;

						flag = 1 ;
						/// 用户成功登陆,同时要修改文件内容，改变该用户的状态以及套接字
						chater[i].state = 1 ;
						chater[i].socket = conn_fd ;
					} else {
						/// 用户已登录
						lemon = 2 ;
					} /* endif the state */
				} else {
					/// 用户密码错误
					lemon = 1 ;
				} /* endif the password */
			} 
			/// 遍历整个用户信息
			i++ ;
		}
		sum = i ;
		fclose (fp) ;
		
		/// 判断是否登陆成功
		if ( !flag ) {
			/// lemon来表示错误原因
			switch ( lemon ) {
				case 0: 
					printf ( "\n\n\t\tSorry, your username is wrong.\n" ) ;
					
					break ;
				case 1:
					printf ( "\n\n\t\tSorry, your password is wrong.\n" ) ;
					
					break ;
				case 2:
					printf ( "\n\n\t\tSorry, the user has been logined.\n" ) ;
					
					break ;
			}
			getchar () ;
			
			return 1 ;
		} else {
			/// 登陆成功
			if ( ( fp = fopen ( "./info" , "wt" ) ) == NULL ) {
				my_err ( "fopen" , __LINE__ ) ;
			}
			
			i = 0 ;
			
			/// 将数据更新重新写入文件
			while ( i < sum  ) {
				fprintf ( fp , "%s %s %d %d\n" , chater[i].username , chater[i].password , chater[i].state , chater[i].socket ) ;
				i++ ;
			}
			
			fclose (fp) ;
			
			break ;
		}
	}
	
	return 0 ;
}


int main ( int argc , char **argv )
{
	int 	conn_fd ;

	system( "touch info" ) ;
	conn_fd = link_server () ;

	getchar () ;

	menu ( conn_fd ) ;

	return 0 ;
}
