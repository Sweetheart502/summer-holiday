int find_receiver ( struct line *head, char *name )
{
	struct line 	*p = head->next;

	while ( p != NULL ) {
		if ( strcmp (name, p->username) == 0 ) {
			break;
		}
	}

	if ( p != NULL ) {
		return p->socket;
	} else {
		return -1;
	}
}
