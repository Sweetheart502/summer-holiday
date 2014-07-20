/*
 * =====================================================================================
 *
 *       Filename:  show.c
 *
 *    Description:  自己的ls,目前仅仅实现-l/-a.
 *
 *        Version:  1.0
 *        Created:  2014年07月19日 11时51分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sweetheart502, liyingxiao502@gmail.com
 *        Company:  Class 1304 of Software Engineering
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define NONE 	0
#define A 	1
#define L 	2
#define R 	4
#define MAXROW 	100

int maxlen;
int leavelen = MAXROW;

//函数声明
void error ( char * err, int line ); 					//报错函数

void show_dir ( int flag, char * path ); 				//目录函数

void file_sort ( char file[][PATH_MAX + 1], int count ); 		//文件排序

void show ( int flag, char *file ); 					//根据参数选取相应的函数

void show_single ( char *name ); 					//展示文件名即可

void show_content ( struct stat buf ); 					//展示文件详细信息


/* 主函数－根据输入参数获得ls的参数 */
int main(int argc, char *argv[])
{
	int 		i, j, k, num;
	char 		path[PATH_MAX + 1]; 				//路径
	char 		param[10]; 					//参数数组
	int  		flag; 						//参数判断标志
	struct stat 	buf; 						//获得文件信息

	j = 0;
	num = 0;
	
	for ( i = 1; i < argc; i++ ) { 					//记录命令行输入的参数内容
		if ( argv[i][0] == '-' ) { 				//用'-'来判断输入的是否参数
			for ( k = 1; k < strlen ( argv[i] ); k++ ) 	//存储参数于参数数组中
			      param[j++] = argv[i][k];
			num++; 						//命令行输入的含'-'参数统计
		}
	}

	for ( i = 0; i < j; i++ ) { 					//根据参数数组的内容，将flag更新
		if ( param[i] == 'a' ) { 				//参数a
			flag |= A;
			continue;
		}
		if ( param[i] == 'l' ) { 				//参数l
			flag |= L;
			continue;
		}
		if ( param[i] == 'r' ) { 				//参数r
			flag |= R;
			continue;
		}
	}
	param[j] = '\0';

	if ( ( num + 1 ) == argc ) { 					//说明命令行没有指定目录，即show当前目录下文件
		strcpy ( path, "./" );
		path[2] = '\0';

		show_dir ( flag, path );
		return 0;
	}

	/* 指定路径的文件或者目录。是文件则直接调用函数show,是目录则调用函数show_dir. */
	i = 1;
	do {
		if ( argv[i][0] == '-' ) {
			i++;
			continue;
		}
		else {
			strcpy ( path, argv[i] );
			/* 检验该路径是否存在 */
			if ( stat ( path, &buf ) == -1 ) {
				error ( "stat", __LINE__ );
			}

			if ( S_ISDIR ( buf.st_mode ) ) { 		//该路径是一个目录
				strcpy ( path, argv[i] );

				if ( path[strlen ( argv[i] ) - 1] != '/' ) {
					path[strlen ( argv[i] )] = '/';
					path[strlen ( argv[i] ) + 1] = '\0';
				}
				else {
					path[strlen ( argv[i] )] = '\0';
				}

				show_dir ( flag, path );
				i++;
			}
			else { 						//该路径是一个文件
				show ( flag, path );
				i++;
			}
		} 
	} while ( i < argc );

	return EXIT_SUCCESS;
}


/* 出错函数 */
void error ( char * err, int line )
{
	fprintf ( stderr, "line: %d\n", line );
	perror ( err );
	exit ( 0 );
}


/* 目录函数－获得含有路径的文件名，获得最长文件名长度，统计文件总数， */
void show_dir ( int flag, char * path )
{
	int 		count = 0; 							//统计即path目录下文件总数
	char 		file[256][PATH_MAX + 1]; 					//存储带路径的文件名
	struct dirent 	* ptr; 								//获得目录下的文件信息
	DIR 		* dir;

	int 		i, j, len;
	char 		temp[PATH_MAX + 1]; 						//临时交换变量

	/* 获得最长文件名和文件总数 */
	dir = opendir ( path );
	if ( dir == NULL ) {
		error ( "opendir", __LINE__ );
	}

	maxlen = 0;
	while ( ( ptr = readdir ( dir ) ) != NULL ) { 					//读取目录信息
		if ( maxlen < strlen ( ptr->d_name ) ) {
			maxlen = strlen ( ptr->d_name ); 				//通过文件名，得出最长文件名maxlen
		}
		count++; 								//统计文件总数count
	}

	closedir ( dir );

	/* 获得带路径的文件名称 */
	dir = opendir ( path );
	if ( dir == NULL ) {
		error ( "opendir", __LINE__ );
	}

	len = strlen ( path );
	for ( i = 0; i < count; i++ ) { 						//遍历整个目录下文件
		ptr = readdir ( dir ); 							//读取文件信息
		if ( ptr == NULL ) {
			error ( "readdir", __LINE__ ); 					//读目录出错
		}

		strcpy ( file[i], path );
		file[i][len] = '\0';

		strcat ( file[i], ptr->d_name );
		file[i][len + strlen ( ptr->d_name )] = '\0';
	}

	// 冒泡法对文件进行排序 
	for ( i = 0; i < count - 1; i++ ) {
		for ( j = 0; j < count - 1 - i; j++ ) {
			if ( strcmp ( file[j], file[j + 1] ) > 0 ) { 			//需要交换
				strcpy ( temp, file[j] );
				temp[strlen ( file[j] )] = '\0';

				strcpy ( file[j], file[j + 1] );
				file[j][strlen ( file[j + 1] )] = '\0';

				strcpy ( file[j + 1], temp );
				file[j + 1][strlen ( temp )] = '\0';
			}
		}
	}

	for ( i = 0; i < count; i++ ) {
		show ( flag, file[i] );
	}

	closedir ( dir );

	/* 看是否含有l参数，没有则输出回车换行 */
	if ( ( flag & L ) == 0 ) {
		printf ( "\n" );
	}
}


/* 获得不含有路径的文件名，获得文件的详细信息 */
void show ( int flag, char *file )
{
	char 		name[PATH_MAX + 1]; 						//去掉路径的文件名
	int 		i, j; 								//作为循环变量来获得文件名
	struct stat 	buf; 								//获得文件信息

	/* 获得单纯文件名 */
	for ( i = 0, j = 0; i < strlen ( file ); i++ ) {
		if ( file[i] == '/' ) {
			j = 0;
			continue;
		}
		
		name[j++] = file[i];
	}
	name[j] = '\0';

	/* 获得文件的详细信息 */
	if ( lstat ( file, &buf ) == -1 ) {
		error ( "lstat", __LINE__ );
	}

	/* 根据参数获得输出文件的格式，调用相应的函数 */
	switch ( flag ) {
		case NONE:
			if ( name[0] != '.' )
				show_single ( name );
			break;
		case A:
			show_single ( name );
			break;
		case L:
			if ( name[0] != '.' ) {
			      show_content ( buf );
			      printf ( "  %s\n", name );
			}
			break;
		case L + A:
			show_content ( buf );
			printf ( "  %s\n", name );
			break;
		default:
			break;
	}
}


/* 单纯地展示文件名称 */
void show_single ( char * name )
{
	int 	i, len;
	//static leavelen = 160;
	len = strlen ( name );
	if ( maxlen > leavelen ) {
		printf ( "\n" );
		leavelen = MAXROW;
	}

	len = maxlen - len; 							//以最长文件名为标准长度，看还需要输入多少个空格
	leavelen = leavelen - maxlen - 2; 					//当前该行剩余字符数

	printf ( "%-s", name );
	for ( i = 0; i < len; i++ )
	      printf ( " " );

	printf ( "  " ); 							//统一的间隔距离
}


/* 展示文件的详细信息 */
void show_content ( struct stat buf )
{
	struct passwd 	* usr; 							//获得文件所有者用户名
	struct group 	* grp; 							//获得文件所有者所属组
	char 		m_time[32]; 		
	//因为由ctime函数所得到的时间中含有"\n",所以用一个字符串来存储去掉"\n"的时间
	
	/* 输出文件类型 */
	if ( S_ISLNK ( buf.st_mode ) ) { 					//符号链接文件
		printf ( "l" );
	}
	if ( S_ISREG ( buf.st_mode ) ) { 					//普通文件
		printf ( "-" ); 
	}
	if ( S_ISDIR ( buf.st_mode ) ) { 					//目录文件
		printf ( "d" );
	}
	if ( S_ISCHR ( buf.st_mode ) ) { 					//字符设备文件
		printf ( "c" );
	}
	if ( S_ISBLK ( buf.st_mode ) ) { 					//块设备文件
		printf ( "b" );
	}
	if ( S_ISFIFO ( buf.st_mode ) ) { 					//管道文件
		printf ( "f" );
	}
	if ( S_ISSOCK ( buf.st_mode ) ) { 					//socket套接字文件
		printf ( "s" );
	}

	/* 输出文件所有者权限 */
	if ( buf.st_mode & S_IRUSR ) {
		printf ( "r" );
	}
	else 
	      printf ( "-" );
	if ( buf.st_mode & S_IWUSR ) {
		printf ( "w" );
	}
	else 
	      printf ( "-" );
	if ( buf.st_mode & S_IXUSR ) {
		printf ( "x" );
	}
	else 
	      printf ( "-" );
	
	/* 输出文件所属组权限 */
	if ( buf.st_mode & S_IRGRP ) {
		printf ( "r" );
	}
	else 
		printf ( "-" );
	if ( buf.st_mode & S_IWGRP ) {
		printf ( "w" );
	}
	else 
		printf ( "-" );
	if ( buf.st_mode & S_IXGRP ) {
		printf ( "x" );
	}
	else 
		printf ( "-" );

	/* 输出其他用户的权限 */
	if ( buf.st_mode & S_IROTH ) {
		printf ( "r" );
	}
	else 
		printf ( "-" );
	if ( buf.st_mode & S_IWOTH ) {
		printf ( "w" );
	}
	else 
	      	printf ( "-" );
	if ( buf.st_mode & S_IXOTH ) {
		printf ( "x" );
	}
	else 
	      	printf ( "-" );

	printf ("  "); 								//统一间隔

	printf ( "%-4u", buf.st_nlink ); 					//文件硬链接数

	usr = getpwuid ( buf.st_uid ); 						//获取文件所有者名
	grp = getgrgid ( buf.st_gid ); 						//获取文件所属组名

	/* 输出文件所有者名及所属组名 */
	printf ( "%-12s", usr->pw_name );
	printf ( "%-12s", grp->gr_name );

	/* 输出文件大小 */
	printf ( "%6ld", buf.st_size );

	/* 获得文件修改时间 */
	strcpy ( m_time, ctime ( &buf.st_mtime ) ); 
	m_time[strlen ( ctime ( &buf.st_mtime ) ) - 1] = '\0';

	/* 输出修改时间 */
	printf ( "  %s", m_time );
}

/*
void file_sort ( char file[][PATH_MAX + 1], int count )
{
	int i, j;
	char temp[PATH_MAX + 1];

	// 冒泡法对文件进行排序 
	for ( i = 0; i < count - 1; i++ ) {
		for ( j = 0; j < count - 1 - i; j++ ) {
			if ( strcmp ( file[j], file[j + 1] ) > 0 ) { 			//需要交换
				strcpy ( temp, file[j] );
				temp[strlen ( file[j] )] = '\0';

				strcpy ( file[j], file[j + 1] );
				file[j][strlen ( file[j + 1] )] = '\0';

				strcpy ( file[j + 1], temp );
				file[j + 1][strlen ( temp )] = '\0';
			}
		}
	}

}
*/
