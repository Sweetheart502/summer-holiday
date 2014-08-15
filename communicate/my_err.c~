//display the error 
void my_err ( char *string, int line )
{
	fprintf ( stderr, "line: %d.\n", line );
	perror ( string );

	//record the error to the file of error
	record_err ( string );

	//make it exit
	exit (1);
}

//record the error to the file of error
void record_err ( char *string )
{
	//the file's discribtion
	int 	fd;

	//open the file 
	if ( ( fd = open ( "./error", O_RDWR|O_CREAT|O_APPEND ) ) == -1 ) {
		perror ( "open" );
		exit (1);
	}

	//write the function name of the error
	record_wr ( fd, string );


	//write the "error"

	if ( write ( fd, " error at ", 10 ) != 10 ) {
		perror ( "write" );
		exit (1);
	}

	//write the time to the file
	record_time ( fd );

	close ( fd );
}

