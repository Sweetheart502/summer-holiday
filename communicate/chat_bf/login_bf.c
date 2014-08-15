
void login ()
{
	int 	conn_fd; 			//the client's socket
	int 	count = 0; 			//do statistics
	char 	send_msg[MAX_LEN]; 		//store the send_msg
	char 	name[32]; 			//receive the username

	//实现客户端与服务器的连接
	conn_fd = link_server ();

	//用户身份验证，三次机会
	while ( count < 3 ) {
		if ( user_login ( conn_fd, name ) == 0 ) {
		      //登陆成功
		      printf ( "\n\n\t\tCongratrulation!\n" );

		      //暂停：使用户看到提示信息
		      getchar ();
		      break;
		}
		else {
		      //登录失败
		      count++;
		}
	
	}

	if ( count == 3 ) {
		printf ( "your have made it three times, so the system command you to exit.\n" );
		
		//暂停：使用户看到提示信息
		getchar ();
		exit (1);
	} 
	
	#ifdef DEBUG
		printf ( "name = %s\n", name );
		getchar ();
	#endif
	//clear the screen
	//system ( "clear" );

	//send the success
	strcpy ( send_msg, name );

	#ifdef DEBUG
		printf ( "send_msg = %s\n", send_msg );
		getchar ();
	#endif

	if ( send ( conn_fd, send_msg, sizeof (send_msg), 0 ) < 0 ) {
		my_err ( "send", __LINE__ );
	}

	if ( send ( conn_fd, &conn_fd, sizeof (int), 0 ) < 0 ) {
		my_err ( "send", __LINE__ );
	}
	

	//否则就是进行聊天communication
}
