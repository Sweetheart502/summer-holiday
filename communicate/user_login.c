//input the user_info and identity its validity
int user_login ( int conn_fd, struct user *new )
{
	//输入用户名和密码，并验证是否存在，存在且未登录则登陆，并修改其状态以及套接字数值．
	FILE 		*fp;
	struct user	chater[20];
	char 		recv_msg[MAX_LEN];
	int 		flag = 0;
	int 		lemon = 0;
	int 		i, sum;


	printf ("socket is %d \n" , conn_fd ) ;

	while (1){
		//进入身份验证页面
		system ( "clear" );
	
		printf ( "\n\t\t---------------------------------------\n" );
		printf ( "\t\t\t ------ WELCOME TO LOGIN ------\n" );

		printf ( "\n\n\n\t\t\t   username: " );
		printf ( "\n\n\t\t\t   password: \33[2A" );
	
		//输入用户昵称
		scanf ( "%s", new->username );
		printf ( "\33[2B\t\t\t             \33[1A" );

		//输入用户密码,使其不回显
		system ( "stty -echo" );
		scanf ( "%s", new->password );
		system ( "stty echo" );
		new->socket = conn_fd ;
		//清除缓存
		getchar ();

		//open the file_info
		if ( ( fp = fopen ( "./info", "rt" ) ) == NULL ) {
			my_err ( "fopen", __LINE__ );
		}

		//设置循环变量并初始化
		i = 0;
		while ( !feof ( fp ) ) {
			//从文件中读出用户信息
			fscanf ( fp, "%s %s %d %d\n", chater[i].username, chater[i].password, &chater[i].state, &chater[i].socket );

			//比较用户信息是否正确
			if ( strcmp ( new->username, chater[i].username ) == 0 ) {
				//用户名匹配
				if ( strcmp ( new->password, chater[i].password ) == 0 ) {
					//用户密码匹配
					if ( chater[i].state == 0 ) {
						//在服务器端显示用户登陆成功
						printf ( "\n\n\t\t" );
						printf ( "%s login", chater[i].username );
						//用户当前不在线
						flag = 1;
						//用户成功登陆,同时要修改文件内容，改变该用户的状态以及套接字
						chater[i].state = 1;
						chater[i].socket = conn_fd;
					} else {
						//用户已登录
						lemon = 2;
					} /* endif the state */
				} else {
					//用户密码错误
					lemon = 1;
				} /* endif the password */
			} 
			//遍历整个用户信息
			i++;
		}
		sum = i;
		fclose (fp);

		//判断是否登陆成功
		if ( !flag ) {
			//lemon来表示错误原因
			switch ( lemon ) {
				case 0: 
					printf ( "\n\n\t\tSorry, your username is wrong.\n" );
	
					break;
				case 1:
					printf ( "\n\n\t\tSorry, your password is wrong.\n" );
					
					break;
				case 2:
					printf ( "\n\n\t\tSorry, the user has been logined.\n" );
					
					break;
			}
			getchar ();

			return 1;
		} else {
			//登陆成功
			if ( ( fp = fopen ( "./info", "wt" ) ) == NULL ) {
				my_err ( "fopen", __LINE__ );
			}
	
			i = 0;
	
			//将数据更新重新写入文件
			while ( i < sum  ) {
				fprintf ( fp, "%s %s %d %d\n", chater[i].username, chater[i].password, chater[i].state, chater[i].socket );
				i++;
			}
	
			fclose (fp);

			break;
		}
	}

	return 0;
}
