void delete_link ( char *string )
{
	struct line *p, *q;

	q = head;
	p = head->next;

	for ( ; p; q = p, p = p->next ) {
		/// 寻找即将下线用户并删除结点
		if ( strcmp (p->username, string ) == 0 ) {
			q->next = p->next;
			free (p);
			break;
		}
	}
}

void list_line ( char *string )
{
	struct line *p = head->next;
 
	strcpy ( string, "\n在线用户: \n" );
	

	while ( p ) {
		/// 将用户名信息写入
		strcat ( string, "\t " );
		strcat ( string, p->username );
		strcat ( string, " \n" );


		/// 遍历在线链表
		p = p->next;
	}
}
