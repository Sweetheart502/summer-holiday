//change the exiter's state and socket
void exit_change ( char *name )
{
	FILE 		*fp;
	int 		i, sum;
	struct user	chater[20];

	//first to read it to here
	if ( ( fp = fopen ( "./info", "rt" ) ) == NULL ) {
		my_err ( "fopen", __LINE__ );
	}

	i = 0;
	while ( fscanf ( fp, "%s %s %d %d\n", chater[i].username, chater[i].password, &chater[i].state, &chater[i].socket ) != EOF ) {
		if ( strcmp ( chater[i].username, name ) == 0 ) {
			chater[i].state = 0;
			chater[i].socket = 0;
		}
		i++;
	}
	sum = i;
	fclose (fp);

	//then to update it to file
	if ( ( fp = fopen ("./info", "wt") ) == NULL ) {
		my_err ( "fopen", __LINE__ );
	}
	
	i = 0;
	while ( i < sum ) {
		fprintf ( fp, "%s %s %d %d\n", chater[i].username, chater[i].password, chater[i].state, chater[i].socket );
		i++;
	}

	fclose (fp);
}
