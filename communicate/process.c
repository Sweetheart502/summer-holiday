#include "thread_chat.c"
#include <assert.h>
//process the communication of the server and the client 
void process ( int sock_fd )
{
	int 			cli_len; 	//客户端地址长度
	pthread_t 		tid; 		//线程ID
	struct sockaddr_in 	cli_addr; 	//客户端地址

	int 			socket , ret;

	struct user 		member;
	struct line 		*p = head, *q;
	
	/// to accept the client's request
	cli_len = sizeof ( struct sockaddr_in );

	if ( (member.socket = accept ( sock_fd, ( struct sockaddr * )&cli_addr, &cli_len ) ) == -1 )	 {
		my_err ( "accept", __LINE__ );
	}

	/// display the ip
	printf ( " \n\t\t  -.- A new client %s is connected. -.-\n\n", inet_ntoa (cli_addr.sin_addr) );

	/// apply a node
	q = ( struct line * ) malloc ( sizeof ( struct line ) );

	/// receive the client's msg
	memset ( member.username, 0, 32 );
	if (( ret = recv ( member.socket, member.username, 32, 0 ) ) < 0 ) {
		my_err ( "recv", __LINE__ );
	}
	
	assert (ret == 32) ;

	strcpy ( q->username, member.username );

	//printf ( "%s\n", q->username );

	q->socket = member.socket;

	/// display the login in the server
	printf ( "\n\t\t---------------------------------------------------" );
	printf ( "\n\t\t\t >_<  username：%s\t socket: %d", member.username, member.socket );
	printf ( "\n\t\t---------------------------------------------------\n" );

	/// login_file
	//login_file (member.username);

	//member.state = 1;

	/// insert the login to the tail
	while ( p->next != NULL ) {
		p = p->next;
	}

	q->next = p->next; 	
	/// 挂尾链
	p = p->next = q;
	/// 挂前链，挪尾指针
	
	/// create a thread to process the command
	pthread_create ( &tid, NULL, (void *)thread_chat, (void*)&member );
}
