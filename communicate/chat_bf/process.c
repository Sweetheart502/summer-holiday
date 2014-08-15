//to process it in the thread
void *thread_chat ( int *conn_fd )
{
	struct user 	user;
	/*
	 * struct user {
	 * 	char 	username[32];
	 * 	char 	password[32];
	 * 	int 	state;
	 * 	int 	socket;
	 * }USER;
	 */
	struct msg 	recv_msg;
	/*
	 * struct msg {
	 * 	char sender[32];
	 * 	char receiver[32];
	 * 	char commmand[32];
	 * 	char msg[MAX_LEN];
	 * }MSG;
	 */
	user.socket = *conn_fd;
	
//	while ( 1 ) {
		int 	ret;
		//init the msg
		memset ( &recv_msg, 0, sizeof (struct msg) );

		//recv the msg from the client
		if ( ( ret = recv ( *conn_fd, &recv_msg, sizeof (struct msg), 0 ) ) < 0 ) {
			my_err ( "recv", __LINE__ );
		}

		//if ( strcmp ( msg.command, "LS" ) == 0 ) {
			//显示在线人数及相关信息　
			//list_login ();
		//} else if {
			//发送信息

		//}
}


//process the communication of the server and the client 
void process ( int sock_fd )
{
	int 			conn_fd; 	//客户端套接字
	int 			cli_len; 	//客户端地址长度
	pthread_t 		tid; 		//线程ID
	struct sockaddr_in 	cli_addr; 	//客户端地址
	char 			name[MAX_LEN];
	int 			socket;
	char 			send_msg[MAX_LEN];

	struct line 		*p = head, *q;
	//to accept the client's request
	cli_len = sizeof ( struct sockaddr_in );

	if ( (conn_fd = accept ( sock_fd, ( struct sockaddr * )&cli_addr, &cli_len ) ) == -1 )	 {
		my_err ( "accept", __LINE__ );
	}

	//display the ip
	printf ( " \n\t\t >_< A new client %s is connected.\n\n", inet_ntoa (cli_addr.sin_addr) );

	//apply a node
	q = ( struct line * ) malloc ( sizeof ( struct line ) );

	//receive the client's msg
	memset ( name, 0, sizeof (name) );
	if ( recv ( conn_fd, name, sizeof (name), 0 ) < 0 ) {
		my_err ( "recv", __LINE__ );
	}
	strcpy ( q->username, name );

/*
	if ( recv ( conn_fd, &socket, sizeof (int), 0 ) < 0 ) {
		my_err ( "recv", __LINE__ );
	}
	q->socket = socket;
	#ifdef DEBUG
		printf ( "socket = %d\nq->socket = %d\n", socket, q->socket );
	#endif
*/
	q->socket = conn_fd;

	//insert the login to the tail
	while ( p->next != NULL ) {
		p = p->next;
	}
	q->next = p->next; 	//挂尾链
	p = p->next = q; 	//挂前链，挪尾指针
	
	//create a thread to process the command
	pthread_create ( &tid, NULL, (void *)thread_chat, &conn_fd );
}
