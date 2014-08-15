void display ( int conn_fd )
{
	int 		choice;
	int 		flag;
	struct msg 	info;


	while (1) {
		system ( "clear" );

	//	getchar ();

		printf ( "\t\t ++ FREE WORLD ++ \n\n" );
		printf ( "\t  ONE: list the member that online.\n\n" );
		printf ( "\t  TWO: chatting with fellow.\n\n" );
		printf ( "\t  THREE: exit the system.\n\n" );
		printf ( "\t--------------------------------\n\n" );

		printf ( "\t\tCHOICE: " );
		scanf ( "%d", &choice );
		printf ( "\n\n" );

		flag = 0;
		switch ( choice ) {
			case 1:
				strcpy ( info.command, "list" );
				flag = 1;
				break;
			case 2:
				flag = 1;
				break;
			case 3:
				flag = 1;
				break;
			default:
				printf ( "Please according to the tip to input angin.\n\n" );
				break;
		}

		if ( flag ) {
			break;
		}
	}

	getchar ();

	if ( send ( conn_fd, &info, sizeof (struct msg), 0 ) < 0 ) {
		my_err ( "send", __LINE__ );
	}

}

//实现客户端与服务器之间的交流
void communicate ( int conn_fd )
{
	getchar ();
	//struct msg 		info;

	display ( conn_fd );
}

