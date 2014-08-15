//#include "head.h"
//#include "my_err.c"

int link_server ()
{
	//用户选择登陆之后，先进行与服务器的连接
	int 			conn_fd; 		//the client's socket
	int 			serv_port;
	char 			ip[32];
	struct sockaddr_in 	serv_addr;
	int 			count = 0;

	//init the serv_addr
	memset ( &serv_addr, 0, sizeof (struct sockaddr_in) );
	serv_addr.sin_family = AF_INET;
	
	//input the two arguments
	printf ( "please input the port and the ip:\n" );
	scanf ( "%d %s", &serv_port, ip );

	//check the argument's validity
	//serv_port = atoi ( serv_port );
	if ( serv_port < 0 || serv_port > 65535 ) {
		printf ( "invalid port.\n" );
		exit (1);
	} else {
		serv_addr.sin_port = htons ( serv_port );
	}


	if ( inet_aton ( ip, &serv_addr.sin_addr ) < 0 ) {
		printf ( "invalid ip.\n" );
		exit (1);
	}

	#ifdef DEBUG
	printf ( "%s\n", ip );
	#endif

	//creat the conn_fd
	printf ( "Now the client is creating the socket \n" );
	conn_fd = socket ( AF_INET, SOCK_STREAM, 0 );
	if ( conn_fd == -1 ) {
		my_err ( "socket", __LINE__ );
	}
	
	//be connected with the server
	printf ( "Now the client is connected with the server ~\n" );
	if ( connect ( conn_fd, ( struct sockaddr * )&serv_addr, sizeof ( struct sockaddr) ) == -1 ) {
		my_err ( "connect", __LINE__ );
	}
	
	return conn_fd;
}
