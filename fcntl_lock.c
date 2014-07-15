/*
 * =====================================================================================
 *
 *       Filename:  fcntl_lock.c
 *
 *    Description:  练习对锁的应用实例
 *
 *        Version:  1.0
 *        Created:  2014年07月14日 19时46分03秒
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
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

void my_err (char *err_string, int line)
{
	fprintf (stderr, "line: %d \n", line);
	perror (err_string);
	exit (0);
}

/* 测试锁，只有当测试发现参数lock指定的锁能被设置时，返回0 */
int lock_test (int fd, struct flock * lock)
{
	//F_GETLK 如果锁能被设置成功，该命令并不真的设置锁，而只是修改lock的l_type为F_UNLCK，然后返回。
	if (fcntl (fd, F_GETLK, lock) == 0) { 				//执行成功返回0
		if (lock->l_type == F_UNLCK) { 				//测试发现能按参数lock要求设置锁
			printf ("lock can be set in fd\n");
			return 0;
		}
		else { 						//有不兼容的锁存在，打印出设置该锁的进程ID
			if (lock->l_type == F_RDLCK) {
				printf ("can't set lock, read lock has been set by: %d\n", lock->l_pid);
			}
			else if (lock->l_type == F_WRLCK) {
				printf ("can't set lock, write lock has been set by: %d\n", lock->l_pid);
			}
			return -2;
		} 
	}
	else { 							//执行失败返回-1
		perror ("get incompatible locks fail");
		return -1;
	} 
}

/*锁的设置和释放函数*/
int lock_set (int fd, struct flock * lock)
{
	if (fcntl (fd, F_SETLK, lock) == 0) { 				//执行成功返回0
		if (lock->l_type == F_RDLCK) {
			printf ("set read lock, pid: %d\n", getpid ());
		}
		else if (lock->l_type == F_WRLCK) {
			printf ("set write lock, pid: %d\n", getpid ());
		}
		else if (lock->l_type == F_UNLCK) {
			printf ("release lock, pid: %d\n", getpid ());
		}
	}
	else { 							//执行失败返回-1
		perror ("lock operation fail\n");
		return -1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int fd;
	int ret;
	struct flock lock;

	char read_buf[32]; 						//相当于存储字符的缓冲区

	/*打开或者创建文件*/
	//open成功调用会返回一个文件描述符，若有错误发生则返回-1
	if ((fd = open ("example_lock", O_CREAT | O_TRUNC | O_RDWR)) == -1) {
		my_err ("open", errno);
	}

	if (write (fd, "test lock", 10) != 10) {
		my_err ("write", errno);
	}

	/*初始化lock结构*/
	//memset (void *s, int ch, size_t n); 将s中前n个字节用ch替换并返回s，在此处用0替换即实现初始化的目的
	memset (&lock, 0, sizeof (struct flock));

	lock.l_start = SEEK_SET;
	lock.l_whence = 0;
	lock.l_len = 0;

	/*设置读锁*/
	lock.l_type = F_RDLCK;
	
	if (lock_test (fd, &lock) == 0) { 				//测试可以设置锁
		lock.l_type = F_RDLCK;
		lock_set (fd, &lock);
	}

	/*读数据*/
	lseek (fd, 0, SEEK_SET);
	
	if ((ret = read (fd, read_buf, 10)) == -1) { 			//read()成功返回字符数，失败返回-1
		my_err ("read", errno);
	}

	read_buf[ret] = '\0';
	printf ("%s\n", read_buf);

	/*等待任意按键*/
	getchar ();

	/*设置写锁*/
	lock.l_type = F_WRLCK;
	
	if (lock_test (fd, &lock) == 0) { 				//测试可以设置锁
		lock.l_type = F_WRLCK;
		lock_set (fd, &lock);
	}

	/*释放锁*/
	lock.l_type = F_UNLCK;
	lock_set (fd, &lock);

	close (fd);

	return EXIT_SUCCESS;
}

