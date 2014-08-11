//注册用户并将其写入用户信息文件
void regist ()
{
	FILE 	*fp;
	int 	i, flag;
	struct user new, old[20];

	while (1) {

		system ( "clear" );

		printf ( "\t\t ==>== WELCOME TO REGIST ==<==\n" );
		printf ( "\t\t  input username & password\n" );;
	
		scanf ( "\t\t%s %s", new.username, new.password );
		getchar ();

		//open the file_info
		if ( ( fp = fopen ("./info", "rt" ) ) == NULL ) {
			my_err ( "fopen", __LINE__ );
		}

		//read the file_info and to check the new user
		i = 0;
		flag = 0;
		while ( fscanf ( fp, "%s %s %d %d\n", old[i].username, old[i].password, &old[i].state, &old[i].socket ) != EOF ) {
			if ( !strcmp ( new.username, old[i].username )  ) {
				printf ( "\n\nthe name is existed.\n" );
				//the name have been existed
				flag = 1;
				getchar ();
				break;
			}
			i++;
		}
		fclose (fp);
	
		//flag = 0->OK; flag = 1->NO
		if ( flag == 0 ) {
			//init some data
			new.state = 0;
			new.socket = 0;

			//open the file_info again
			if ( ( fp = fopen ("./info", "at") ) == NULL ) {
				my_err ( "fopen", __LINE__ );
			}

			//update the file_info
			fprintf ( fp, "%s %s %d %d\n", new.username, new.password, new.state, new.socket );
			
			fclose ( fp );

			printf ( "\n\n\t\t\tCongratrulation!\n" );
			break;
		}
	}
	getchar ();
}
