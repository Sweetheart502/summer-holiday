#include "delete_link.c"

void record_chat ( char *string )
{
	int 	fd;

	if ( (fd = open ( "./chat",  O_RDWR|O_CREAT|O_APPEND )) == -1 ) {
		my_err ( "open",  __LINE__ );	
	}	

	record_wr ( fd, string );

	//write the time to the file
	record_time ( fd );

}

//to process it in the thread
void *thread_chat ( void * member )
{
	/// 存储用户昵称和用户在服务端的套接字
	struct user 	*user = (struct user *)member; 

	/// 发送信息的结构体
	struct msg 	info, sen;

	/// 接受信息者fd 发送信息者fd 
	int 		send_fd, recv_fd , ret ;



	char 		string[500];

	/// 将在线用户更新
	struct line 	*p = head->next;

	while (1) {
		/// 将结构体清空
		memset ( &info, 0, sizeof (struct msg) );

		/// 接收客户端信息
		if ( ( ret = recv ( user->socket, &info, sizeof (struct msg), 0 ) )< 0 ) {
			my_err ( "recv", __LINE__ );
		}
		
//		assert (ret == sizeof (struct msg)) ;


		/// 在服务器端显示语句
//		printf ( "传过来的数据包： \n" );
		printf ( "%s to %s : %s.\n", info.from, info.to, info.content );

		/// 将聊天信息写入文件chat
		strcpy ( string, info.from );
		strcat ( string, " to " );
		strcat ( string, info.to );
		strcat ( string, " : " );
		strcat ( string, info.content );
		strcat ( string, " \0" );

		record_chat ( string );

		/// 如果是server则判断是不是ls or quit，是则调用相应的函数，不是给它发送输入错误．

		if ( strcmp ( info.to , "server" ) == 0 ) {
			/// 内容识别
			if ( strcmp ( info.content , "list" ) == 0 ) {

				/// 将sen传过去，获得在线用户的字符串
				list_line ( sen.content ) ;
			} else if ( strcmp ( info.content , "quit" ) == 0 ) {

				strcpy ( sen.content , "the user is deleted.\n\0" );
				/// 将该用户从在线链表中删除
				delete_link ( info.from );
			} else {

				/// 说明发送命令有误
				strcpy ( sen.content , "Your command does not exit.\n" );
			}

	
			p = head->next;

			while ( p != NULL ) {
				/// 寻找发送消息者的套接字
				if ( strcmp (p->username, info.from) == 0 ) {
					recv_fd = p->socket;
					break;
				}
				p = p->next;
			}

			strcpy ( sen.to , info.from );
			strcpy ( sen.from , "server" );
			/// 发送信息给发送消息者
			if ( send ( recv_fd, &sen, sizeof ( struct msg ), 0 ) < 0 ) {
				my_err ( "send" , __LINE__ ) ;
			}

		} else {

			p = head->next;
			/// 寻找显现用户中是否存在info.to
			while ( p != NULL ) {
				if ( strcmp ( p->username, info.to ) == 0 ) {
					/// 寻找接收消息者的套接字
					send_fd = p->socket;
					break;
				}
				p = p->next;
			}

			if ( p == NULL ) {
				strcpy ( sen.content, "您输入的用户不在线或者不存在．\n" );
				
				record_chat ( sen.content );

				p = head->next;

				while ( p != NULL ) {
					/// 寻找发送消息者的套接字
					if ( strcmp (p->username, info.from) == 0 ) {
						recv_fd = p->socket;
						break;
					}
					p = p->next;
				}

				strcpy ( sen.to , info.from ) ;
				strcpy ( sen.from , "server" ) ;

				if ( send ( recv_fd, &sen, sizeof ( struct msg ), 0 ) < 0 ) {
					my_err ( "send", __LINE__ );
				}

			} else {
				/// 发送消息给接收消息者
				if ( send ( send_fd, &info, sizeof ( struct msg ), 0 ) < 0 ) {
					my_err ( "send", __LINE__ );
				}
			}

		}
	}

}

