/*
 * =====================================================================================
 *
 *       Filename:  my_ls.c
 *
 *    Description:  实现ls的功能(仅仅可以针对-a，-l两个参数的实现),针对于当前目录下文件的显示.
 *
 *        Version:  1.0
 *        Created:  2014年07月17日 10时25分07秒
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
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <linux/limits.h>

#define PARAM_NONE 	0 				//无参数
#define PARAM_A 	1 				//-a; 显示所有文件
#define PARAM_L 	2 				//-l; 一行只显示一个文件的详细信息
#define MAXROWLINE 	150				//一行显示的最大字符数

int g_leave_len = MAXROWLINE; 				//一行剩余长度，用于输出对齐
int g_maxlen; 						//存放某目录下最长文件名的长度

/* 输出错误信息函数 */
void my_err (char *err_string, int line)
{
	fprintf (stderr, "line: %d\n", line);
	perror ("err_string");
	exit (0);
}

/* 在没有使用-l参数的选项时，打印一个文件名，打印时上下对齐 */
void display_single (char *name)
{
	int i, len;

	//如果本行不足以打印一个文件名则换行
	if (g_leave_len < g_maxlen) {
		printf ("\n");
		g_leave_len = MAXROWLINE;
	}

	len = strlen (name);
	len = g_maxlen - len; 					//以最长文件名为标准还剩下多少长度
	printf ("%-s", name);

	for (i = 0; i < len; i++) {
		printf (" ");
	}
	printf ("  "); 						//在输出两个空格，这两个空格是为了统一间隔而设定的

	g_leave_len -= g_maxlen + 2; 				//更新该行当前剩下的字符数
}

/* 获取文件属性并打印 */
void display_attribute (struct stat buf, char *name)
{
	char 		buf_time[32];
	struct passwd 	*psd; 					//从该结构体中获取文件所有者的用户组
	struct group 	*grp; 					//从该结构体中获取文件所有者所属组的组名

	/* 获取并打印文件类型 */
	if (S_ISLNK (buf.st_mode)) {
		printf ("l");
	}
	else if (S_ISREG (buf.st_mode)) {
		printf ("-");
	}
	else if (S_ISDIR (buf.st_mode)) {
		printf ("d");
	}
	else if (S_ISCHR (buf.st_mode)) {
		printf ("c");
	}
	else if (S_ISBLK (buf.st_mode)) {
		printf ("b");
	}
	else if (S_ISFIFO (buf.st_mode)) {
		printf ("f");
	}
	else if (S_ISSOCK (buf.st_mode)) {
		printf ("s");
	}
	
	/* 获取并打印文件所有者的权限 */
	if (buf.st_mode & S_IRUSR) {
		printf ("r");
	}
	else 
		printf ("-");

	if (buf.st_mode & S_IWUSR) {
		printf ("w");
	}
	else 
		printf ("-");

	if (buf.st_mode & S_IXUSR) {
		printf ("x");
	}
	else 
		printf ("-");

	/* 获取并打印与文件所有者同组的用户权限 */
	if (buf.st_mode & S_IRGRP) {
		printf ("r");
	}
	else 
		printf ("-");

	if (buf.st_mode & S_IWGRP) {
		printf ("w");
	}
	else 
		printf ("-");

	if (buf.st_mode & S_IXGRP) {
		printf ("x");
	}
	else 
		printf ("-");

	/* 获取并打印其他用户对该文件的操作权限 */
	if (buf.st_mode & S_IROTH) {
		printf ("r");
	}
	else 
		printf ("-");

	if (buf.st_mode & S_IWOTH) {
		printf ("w");
	}
	else 
		printf ("-");

	if (buf.st_mode & S_IXOTH) {
		printf ("x");
	}
	else 
		printf ("-");

	printf ("  "); 							//为了统一对齐而设置的间隔

	/* 根据uid和gid获取文件所有者的用户名和用户组名 */
	psd = getpwuid (buf.st_uid);
	grp = getgrgid (buf.st_gid);

	printf ("%-4u", buf.st_nlink); 					//打印文件的硬链接数

	printf ("%-12s", psd->pw_name);
	printf ("%-12s", grp->gr_name);

	printf ("%6ld", buf.st_size); 					//打印文件的大小

	strcpy (buf_time, ctime (&buf.st_mtime));
	buf_time[strlen (buf_time) - 1] = '\0'; 			//去掉换行符，用结束标志来替代它

	printf (" %s", buf_time); 					//打印修改时间
}

/*
 * 根据命令行参数和完整路径名显示目标文件
 * 参数flag: 		命令行参数
 * 参数pathname： 	包含了文件的路径名
 */
void display (int flag, char *pathname)
{
	int i, j;
	struct stat 	buf;
	char name[PATH_MAX + 1];

	/* 从路径中解析出文件名 */
	for (i = 0, j = 0; i < strlen (pathname); i++) {
		if (pathname[i] == '/') {
			j = 0;
			continue;
		}

		name[j++] = pathname[i];
	}
	name[j] = '\0';

	/* lstat函数来方便解析链接文件,获取文件的状态信息 */
	if (lstat (pathname, &buf) == -1) {
		my_err ("lstat", errno);
	}
	/* 
	 * 参数pathname指定文件的路径。
	 * 参数buf表示存储stat结构的缓冲区的地址。
	 * stat结构用于存储文件的状态信息。
	 */

	switch (flag) {
		case PARAM_NONE: 	//没有-l和-a选项
			if (name[0] != '.') 
			      display_single (name);
			break;
		case PARAM_A: 		
			display_single (name);
			break;
		case PARAM_L:
			//printf ("total  %d\n", count);
			if (name[0] != '.') {
				display_attribute (buf, name);
				printf ("  %-s\n", name);
			}
			break;
		case PARAM_A + PARAM_L:
			//printf ("total  %d\n", count);
			display_attribute (buf, name);
			printf ("  %-s\n", name);
			break;
		default:
			break;
	}
}

/* 显示当前目录信息函数 */
void display_dir (int flag_param, char *path)
{
	DIR 		*dir; 					//打开目录获得的目录流
	struct dirent 	*ptr; 					//存储目录信息的结构体
	int 		count = 0; 				//统计当前目录下的文件数目
	char 		filenames[256][PATH_MAX + 1], temp[PATH_MAX + 1];

	int i, j, len;

	//获得当前目录下的文件中最长文件名以及文件数目
	dir = opendir (path); 					//打开目录
	if (dir == NULL) {
		my_err ("opendir", errno);
	}

	g_maxlen = 0; 						//g_maxlen初始化
	while ((ptr = readdir (dir)) != NULL) { 		//读取目录下信息
		if (g_maxlen < strlen (ptr->d_name)) {
			g_maxlen = strlen (ptr->d_name); 	//改变当前的最大文件长度
		}
		count++; 					//统计当前目录下文件数目
	}

	//printf ("%d\n", g_maxlen);

	closedir (dir);

	if (count > 256) {
		printf ("too many files under this dir: %d", errno);
	}

	len = strlen (path);

	//获取当前目录下所有的文件名，将他们存储在filenames数组中
	dir = opendir (path);

	if (dir == NULL) {
		my_err ("opendir", errno);
	}

	for (i = 0; i < count; i++) {
		ptr = readdir (dir); 				//获取文件名称
		
		if (ptr == NULL) {
			my_err ("readdir", errno);
		}

		strcpy (filenames[i], path);
		filenames[i][len] = '\0';
		strcat (filenames[i], ptr->d_name);
		filenames[i][len + strlen (ptr->d_name)] = '\0';
	} /* 存储含当前路径的文件名于filenames中 */

	/* 冒泡法对文件名进行排序，排序后按照当前顺序存储在数组中 */
	for (i = 0; i < count - 1; i++) {
		for (j = 0; j < count - 1 - i; j++) {
			if (strcmp (filenames[j], filenames[j + 1]) > 0) {
				strcpy (temp, filenames[j]);
				temp[strlen (filenames[j])] = '\0';

				strcpy (filenames[j], filenames[j + 1]);
				filenames[j][strlen (filenames[j + 1])] = '\0';
				
				strcpy (filenames[j + 1], temp);
				filenames[j + 1][strlen (temp)] = '\0';
			}
		}
	}
	if (flag_param & PARAM_L) 
		printf ("total  %d\n", count);

	for (i = 0; i < count; i++) {
		//调用函数显示每个文件的信息
		display (flag_param, filenames[i]);
	}

	closedir (dir);

	/* 如果命令行没有-l的选项，则输出一个回车个换行 */
	if ((flag_param & PARAM_L) == 0) {
		printf ("\n");
	}
 } 


int main(int argc, char *argv[])
{
	int 		i, j, k, num; 				//num统计“-”的个数
	char 		path[PATH_MAX +1];
	char 		param[32]; 				//保存命令行参数，目标文件和目录名不在此
	int 		flag_param = PARAM_NONE; 		//参数种类，即是否有-l和-a选项
	struct stat 	buf;

	//获取命令参数，并将它们存储在param数组中，以及获得'-'的数目
	j   = 0;
	num = 0;

	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') { 			//此时需要存储参数,统计'-'参数的个数
			for (k = 1; k < strlen (argv[i]); k++) {
				param[j++] = argv[i][k];
		 	}
			num++; 					//保存'-'的个数
		}
	}

	/* 根据param数组中的参数来获得系统中相应的宏表示，将他们用'|'连接起来 */
	for (i = 0; i < j; i++) {
		if (param[i] == 'a') {
			flag_param |= PARAM_A;
			continue;
		}
		else if (param[i] == 'l') {
			flag_param |= PARAM_L;
			continue;
		}
		else {
			printf ("my_ls: invalid option -%c\n", param[i]);
			exit (1);
		}
	}
	param[j] = '\0'; 					//为参数字符数组添加结束标志

	/* 如果没有输入具体的文件名，则就是显示当前目录下文件信息 */
	if (num + 1 == argc) {
		strcpy (path, "./"); 				//将表示当前目录的字符串写入path中
		path[2] = '\0'; 				//为path添加结束标志
		
		//调用函数，显示文件信息
		display_dir (flag_param, path); 		//将ls参数和显示目录名传过去

		return 0;
	}

	return EXIT_SUCCESS;
}
