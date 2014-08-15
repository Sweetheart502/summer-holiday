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
		printf ( "\t\t -----   WELCOME TO THE CLIENT  -----\n" );
		printf ( "\t\t 			A: regist\n" );
		printf ( "\t\t 			B: login\n" );
		printf ( "\t\t 			C: exit\n" );
		printf ( "\t\t ----------- ENJOY YOURSELF -----------\n" );
		//printf ( "" );
		printf ( "please input your choise:" );

		//输入选项
		scanf ( "%c", &choice );

		//switch结构　客户做出选择
		switch ( choice ) {
			case 'A':
				//用户注册
				regist ();
				system ( "cat info" );
				getchar ();
				break;
			case 'B':
				//用户登录
				login ( conn_fd );
				break;
			case 'C':
				//用户退出
				printf ( "\n\n\t\tI just came to have a look, so I need to leave.\n " );
				exit (0);
			default :
				//出错循环
				printf ( "Your choice isn't exist, please try again.\n" );
				getchar ();
		}
	}
}
