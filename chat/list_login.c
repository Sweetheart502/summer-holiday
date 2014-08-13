void list_login ( struct tid *arg, struct msg *recv_msg )
{
	struct line 	*p = arg->head;

	p = p->next;

	while ( p != NULL ) {
		//send one user_login's info
		if ( send ( arg->conn_fd, p->username, 32, 0 ) < 0 ) {
			my_err ( "send", __LINE__ );
		}
	}

	//send the end to client
	if ( send ( arg->conn_fd, "end", 3, 0 ) < 0 ) {
		my_err ( "send", __LINE__ );
	}
}
