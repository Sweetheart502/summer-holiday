/*
 * =====================================================================================
 *
 *       Filename:  my_rwl.c
 *
 *    Description:  演示文件读写和文件读写指针的移动操作过程
 *
 *        Version:  1.0
 *        Created:  2014年07月14日 11时32分24秒
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
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void my_err (char *err_string, int line)
{
	fprintf (stderr, "line: %d ", line); 		//将错误信息存储于stderr中
	perror (err_string); 				//显示函数发生错误的原因
}

int my_read (int fd)
{
	int len;
	int ret;
	int i;
	char read_buf[64];

	/*获取文件长度，并且使得文件指针在文件开头*/
	if (lseek (fd, 0 ,SEEK_END) == -1) {
		my_err ("lseek", errno);
	}
	if ((len = lseek(fd, 0, SEEK_CUR)) == -1) { 			//调用成功时返回当前的读写位置，即距离文件开始处多少个字节
		my_err ("lseek",errno);
	}
	if (lseek (fd, 0, SEEK_SET) == -1) { 				//使文件指针指在开始位置处
		my_err ("lseek", errno);
	}

	printf ("len: %d\n", len);
	/*读数据*/
	if ((ret = read (fd, read_buf, len)) < 0) { 			//read顺利返回读到的字符个数
		my_err ("read", errno);
	}

	/*打印数据*/
	for (i = 0; i < len; i++) {
		printf ("%c", read_buf[i]);
	}
	printf ("\n");

	printf ("ret = %d\n", ret);

	return ret;
}

/*linux C编程实战  p150 左下角*/

/* od -c example_63.c
 * 使用od命令观察该文件的实际内容。命令行中的-c标志表示以字符方式打印文件的内容。
 * 从中可以看出，文件中间的十个未写字节都被读成为0。
 *每一行开始的一个7位数是以八进制形式表示的字节偏移量。
 */

int main(int argc, char *argv[])
{
	int fd;
	char write_buf[32] = "hello word!"; 				//写入缓冲区内容
	
	/*在当前目录下创建一个example_63.c的文件*/
	//if ((fd = creat ("example_63.c", S_IRWXU)) == -1) { 		//creat 只能以只写的方式打开创建的文件，creat 无法创建设备文件(用mknod函数)
	if ((fd = open ("example_rwl", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU)) == -1) {
		my_err ("open", errno);
	}
	else {
		printf ("creat the file cuccess.\n");
	}

	/*写数据*/
	if (write (fd, write_buf, strlen (write_buf)) != strlen (write_buf)) {
		my_err ("write", errno);
	}
	/*my_read()先测试由参数fd传入的文件描述符对应的文件长度，然后读出所有数据，最后打印出来*/
	my_read (fd);

	/*演示文件的间隔*/
	printf ("/*--------------------------------*/\n");
	if (lseek (fd, 10, SEEK_END) == -1) { 				//使文件指针在文件末尾10个字符处
		my_err ("lseek", errno);
	}
	if (write (fd, write_buf, strlen (write_buf)) != strlen (write_buf)) {
		my_err ("write", errno);
	}
	my_read (fd);

	close (fd);

	return EXIT_SUCCESS;
}

