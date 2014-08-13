#include "exit_change.c"

//实现客户端与服务器之间的交流
void communicate ( int conn_fd, char *name )
{
	char 		choice;
	struct msg 	send_msg;
	char 	 	ls_user[32];
	int 		i;
	
	while (1) {
		system ( "clear" );
/*
		//first display the online_members

		//copy the command
		strcpy ( send_msg.command, "LS" );
		//copy the sender's name
		strcpy ( send_msg.sender, name );
		//send the msg
		if ( send ( conn_fd, &send_msg, sizeof (struct msg), 0 ) < 0 ) {
			my_err ( "send", __LINE__ );
		}
*/
	
		//display some choice
		printf ( "\n\t\tWELCOME TO COMMUNICATE\n" );
		printf ( "\tLS (L) \n" );
		printf ( "\tCHAT (C) \n" );
		printf ( "\tQUIT (Q) \n" );
		printf ( "________``=``_________\n" );
	
		//input the choice
		printf ( "Please input your choice:___\n" );
		printf ( "                          \33[1A" );
		scanf ( "%c", &choice );
	
		//according to the choice to do some thing
		switch ( choice ) {
			case 'L':
				
				strcpy (send_msg.receiver, name);
				strcpy (send_msg.command, "L");

				if ( send ( conn_fd, &send_msg, sizeof (struct msg), 0 ) < 0 ) {
					my_err ( "send", __LINE__ );
				}

				i = 0;
				while ( 1 ) {
					if ( recv ( conn_fd, ls_user, 32, 0 ) < 0 ) {
						my_err ( "recv", __LINE__ );
					}

					if ( strcmp ( "end", ls_user ) == 0 ) {
						printf ( "TOTAL: %d members.\n", i );
						break;
					} else {
						i++;
						printf ( "%s\n", ls_user );
					}
				}

				break;
			case 'C':
//				if ( send ( conn_fd,  ) )
			case 'Q':
				//choose to exit, you need to tell server that you will exit and change your info (state and socket)
	


		//		strcpy ( send_msg. )
				//send the exit
				if ( send ( conn_fd, "exit", 4, 0 ) < 0 ) {
					my_err ( "send", __LINE__ );
				}
	
				//tell server, server delete it from the login_link
				if ( send ( conn_fd, &conn_fd, sizeof (int), 0 ) < 0 ) {
					my_err ( "send", __LINE__ );
				}
	
				//change his info
				exit_change ( name );
	
				close (conn_fd);
				exit (0);
		}
	}
}
