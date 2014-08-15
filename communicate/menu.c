//the client's menu_function
void menu ( int conn_fd )
{
	//显示客户端刚开始一进来的选项：注册　登陆　退出
	char 	choice;
	//int 	conn_fd;

	//connect with the server
	//conn_fd = link_server ();

	//getchar ();

	while (1) {
		//清屏
		system ( "clear" );

		//the client's menu
		printf ( "\t\t -----   WELCOME TO THE CLIENT  -------\n" );
		printf ( "\t\t --------------------------------------\n" );
		printf ( "\t\t 		A: SIGN UP\n\n" );
		printf ( "\t\t 		B: LOG IN\n\n" );
		printf ( "\t\t 		C: QUIT\n\n" );
		printf ( "\t\t --------------------------------------\n" );
		printf ( "\t\t ----------- ENJOY YOURSELF -----------\n" );
		//printf ( "" );
		printf ( "\t\t please input your choice: " );

		//输入选项
		scanf ( "%c", &choice );

		//switch结构　客户做出选择
		switch ( choice ) {
			case 'A':
				//用户注册
				regist ();
//				system ( "cat info" );
				getchar ();
				break;
			case 'B':
				//用户登录
				login ( conn_fd );
				break;
			case 'C':
				//用户退出
				printf ( "\n\n\t\t You have successfully quit the chat.\n " );
				exit (0);
			default :
				//出错循环
				printf ( "Your CHOICE does not exist, please enter again.\n" );
				getchar ();
		}
	}
}
