#include "head.h"
#include "menu.c"
#include "my_err.c"
#include "regist.c"
#include "login.c"
#include "link.c"
#include "user_login.c"

int main ( int argc, char **argv )
{
	int 	conn_fd;

	conn_fd = link_server ();

	getchar ();

	menu ( conn_fd );

	return 0;
}
