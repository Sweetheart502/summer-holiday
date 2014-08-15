#include "exit_change.c"

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
	struct msg  rec ;
	int 	    ret ;

//	printf ("user name %s , socket %d \n" , user ->username , user -> socket) ;

	while ( 1 ) {
		memset ( &rec , 0 , sizeof (struct msg) ) ;

		if ( ( recv ( user->socket , &rec , sizeof (struct msg) , 0 ) )< 0 ) {
		my_err ( "recv" , __LINE__ ) ;
		}
		
		printf ( "%s : %s\n" , rec.from , rec.content ) ;
	}

}

void *send_c ( void *arg ) 
{
	struct user 	*usr = (struct user *)arg ;
	struct msg 	sen ;
	char 		quit[10];

//	printf ("user->username = %s \n" , usr->username) ;


//	printf ("~~~~~username is %s\n" , sen.from) ;
	while ( 1 ) {
		memset ( &sen , 0 , sizeof (struct msg) ) ;
		
		strcpy ( sen.from , usr->username) ;
		
		/// 输入您所要聊天的对象
		printf ( "Please enter the receiver : " );
		scanf ( "%s" , sen.to ) ;
	
		getchar ();
		/// system ( "clear" );

		printf ( "\n\t\t -------------------------------------------\n" ) ;
		printf ( "\n\t\t           You will chat with %s .          \n" , sen.to ) ;
		printf ( "\n\t\t        WANT TO QUIT , ENTER QUIT .         \n" ) ;
		printf ( "\n\t\t -------------------------------------------\n" ) ;

		printf ( "%s: " , sen.from ) ;

		while ( 1 ) {
	
			/// 将发送者昵称显示
			/// printf ( "%s: " , sen.from ) ;
			/// 输入发送消息内容
			scanf ( "%s" , sen.content ) ;

			getchar ();
			
			if ( strcmp ( sen.content , "quit" ) == 0 ) {
				break ;
			} else {
			
				/// 发送数据包
				if ( send ( usr->socket , &sen , sizeof (struct msg) , 0 ) < 0 ) {
					my_err ( "send" , __LINE__ ) ;
				}
			}
		}

		printf ( " 您已经退出与%s聊天，请重新输入您所要聊天的对象 .\n ", sen.to ) ;

		printf ( "  __________>|< CONTINUE or QUIT >|<_____________  \n" ) ;

		scanf ( "%s" , quit );

		getchar ();

		if ( strcmp ( quit , "quit" ) == 0 ) {

			/// should send quit to server , and the serner delete the user
			strcpy ( sen.content , "quit" ) ;
			strcpy ( sen.to , "server" ) ;

			if ( send ( usr->socket , &sen , sizeof ( struct msg ) , 0 ) < 0 ) {
				my_err ( "sen" , __LINE__ ) ;
			}

			exit_change ( sen.from ) ;
			break;
		}
	}

}


void login (int conn_fd)
{
	char 		name[32] ;
	pthread_t 	tid1 ;
	pthread_t 	tid2 ;
	struct user 	new ;

	identify (&new , conn_fd) ;


	pthread_create ( &tid2 , NULL , send_c , (void*)&new ) ;
	pthread_create ( &tid1 , NULL , recv_c , (void*)&new ) ;


	sleep (10000) ;
}
