/*
 * =====================================================================================
 *
 *       Filename:  ls.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年07月17日 18时49分23秒
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
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include <linux/limits.h>

#define NONE 	0
#define A 	1
#define L 	2
#define MAX 	80

int max_len;
int leave_len = MAX;

void my_err (char *err_string, int line) 
{
	fprintf (stderr, "line: %d\n", line);
	perror (err_string);
	exit (0);
}

void display_attribute (struct stat buf, char *name)
{
	char buf_time[32];
	struct passwd *psd;
	struct group *grp;

	if (S_ISLNK ())

}

void display_single (char *name)
{
	int i, len;

	if (leave_len < max_len) {
		printf ("\n");
		leave_len = MAX;
	}

	len = strlen (name);
	len = max_len - len; 	
	printf ("%-s", name);

	for (i = 0; i < len; i++) {
		printf (" ");
	}
	printf ("  ");  //统一间隔
	
	leave_len -= max_len + 2;
}

void display (int flag, char *file)
{
	int i, j;
	struct stat buf;
	char name[PATH_MAX + 1];
	
	for (i = 0, j = 0; i < strlen (file); i++) {
		if (file[i] == '/') {
			j = 0;
			continue;
		}
		name[j++] = file[i];
	}
	name[j] = '\0';

	if (lstat (path, &buf) == -1) {
		my_err ("lstat", errno);
	}
	
	switch (flag) {
		case NONE:
			if (name[0] != '.') {
				display_single (name);
				break;
			}
		case A:
			display_single (name);
			break;
		case L:
			if (name[0] != '.') {
				display_attribute (buf, name);
				printf ("  %-s\n", name);
			}
			break;
		case A+L:
			display_attribute (buf, name);
			printf ("  %-s\n", name);
			break;
		default :
			break;
	}
}

void display_dir (int flag, char *path)
{
	DIR *dir; 	//目录流
	struct dirent *ptr; 	//目录下文件信息
	int count = 0;
	char file[256][PATH_MAX + 1];
	int i, j,len;
	char temp[PATH_MAX + 1];

	dir = opendir (path);
	while ((ptr = readdir (dir)) != NULL) {
		if (strlen (ptr->d_name) > max_len) 
		      max_len = strlen (ptr->d_name);
		count++;
	}
	closedir (dir);

	if (count >256) {
		printf ("too many file.\n");
		exit (0);
	}

	len = strlen (path);
	dir = opendir(path);
	for (i = 0; i < count; i++) {
		ptr = readdir (dir);
		strcpy (file[i], path);
		path[len] = '\0';
		strcat (file[i], ptr->d_name);
		file[strlen (ptr->d_name) + len] = '\0';
	}

	for (i = 0; i <count - 1; i++) {
		for (j = 0; j < count - i -1; j++) {
			if (strcmp (file[j], file[j+1]) > 0) {
				strcpy (temp, file[j]);
				temp[strlen (file[j])] = '\0';

				strcpy (file[j], file[j+1]);
				file[strlen (file[j+1])] = '\0';

				strcpy (file[j+1], temp);
				file[strlen (temp)] = '\0';
			}
		}
	}

	for (i = 0; i < count; i++)
	      display (flag, file[i]);

	closedir (dir);
}

int main(int argc, char *argv[])
{
	int i, j, k,n num;
	char path[PATH_MAX + 1];
	char param[32];
	int flag = NONE;
	struct stat buf; 			//存储文件信息

	j = 0;
	num = 0;
	for (i = 1; i < argc; i++) {
		if (argv[i] == '-') {
			for (k = 0; k < strlen (argv[i]); k++)
			      param[j++] = argv[i][k];
			num++;
		}
	}

	for (i = 0; i < j; i++) {
		if (param[i] == 'a') {
			flag |= A;
			continue;
		}
		else if (param[i] == 'l') {
			flag |= L;
			continue;
		}
		else {
			printf ("the invalid option.\n");
			exit (1);
		}
	}
	param[j] = '\0';

	if (num + 1 == argc) { 			//当前目录
		strcpy (path, "./");
		path[2] = '\0';
		display_dir (flag, path);
		return 0;
	}

	return EXIT_SUCCESS;
}

