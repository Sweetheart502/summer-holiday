//#include "list_login.c"
//#include "find_receiver.c"

void *recv_msg ( ( struct tid * )msg )
{

}

/*
//to process it in the thread
void *recv_msg ( (struct tid *)arg )
{
	struct msg 	recv_msg;
	int 		send_fd;

	while (1) {
		//init
		memset ( &recv_msg, 0, sizeof (struct msg) );

		//receive msg
		if ( recv ( arg->conn_fd, &recv_msg, sizeof (struct msg), 0 ) < 0 ) {
			my_err ( "recv", __LINE__ );
		}

		//according to the command ti choose differernt function
		if ( strcmp ( recv->command, "L" ) == 0 ) {
			list_login ( arg, &recv_msg );
		} else if ( strcmp ( recv_msg->command, "C" ) == 0 ) {
			//find the user and returned the socket
			send_fd = find_receiver ( arg->head, recv_msg.receiver );

	//		if ( send_fd == -1 ) {
				//输入姓名有误或者不在线
	//		}

			//send msg
			if ( send ( send_fd, recv_msg.msg, MAX_LEN, 0 ) < 0 ) {
				my_err ( "send", __LINE__ );
			}

			//这里可以添加聊天日志

			//display the msg
			printf ( "%s: %s\n", recv_msg.sender, recv_msg.msg );
		} else if ( strcmp ( arg->command, "Q" ) == 0 ) {
			printf ( "The client will log out.\n" );
			close (arg->conn_fd);
		}
	}

}
*/

//process the communication of the server and the client 
void process ( int sock_fd, struct line *head)
{
	struct tid 		arg ;
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

	#ifdef DEBUG
		printf ( "conn_fd = %d\n", conn_fd );
	#endif

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
	
	#ifdef DEBUG
		printf ( "name = %s\nq->username = %s\n", name, q->username );
		getchar ();
	#endif
	
	if ( recv ( conn_fd, &socket, sizeof (int), 0 ) < 0 ) {
		my_err ( "recv", __LINE__ );
	}
	q->socket = socket;
	#ifdef DEBUG
		printf ( "socket = %d\nq->socket = %d\n", socket, q->socket );
	#endif
	

//	q->socket = conn_fd;
	
	//insert the login to the tail
	while ( p->next != NULL ) {
		p = p->next;
	}
	q->next = p->next; 	//挂尾链
	p = p->next = q; 	//挂前链，挪尾指针
	
	#ifdef DEBUG
		p = head->next;
		while ( p != NULL ) {
			printf ( "p->username = %s\np->socket = %d\n", p->username, p->socket );
			p = p->next;
		}
	#endif

	arg.conn_fd = conn_fd;
	arg.head = head;
	
	//create a thread to process the command
	pthread_create ( &tid, NULL, (void *)recv_msg, &arg );
}
