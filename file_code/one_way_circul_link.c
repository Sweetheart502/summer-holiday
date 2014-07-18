/*
 * =====================================================================================
 *
 *       Filename:  one_way_circul_link.c
 *
 *    Description:  单向循环链表创建插入删除输出算法的实现。
 *
 *        Version:  1.0
 *        Created:  2014年07月16日 11时58分28秒
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
#define N 5

typedef struct node {
	int data;
	struct node *next;
} ST;

ST * creatlink (int a[])
{
	int i;
	ST *tail = NULL, *p; 					//tail时所设置的尾指针，p是遍历整个链表的指针变量

	for (i = 0; i < N; i++) {
		p = (ST *)malloc(sizeof (ST));
		p->data = a[i];

		if (!tail) {
			//此时表示当前节点是所要创建的第一个结点
			tail->next = tail = p;
		}
		else {
			//直接将p结点插入在尾结点之后成为新的尾结点
			p->next = tail->next;
			tail = tail->next = p;
		}
	}

	return tail;
}

void printlink (ST *tail)
{
	ST *p = tail;

	do {
		printf ("%4d", p->next->data);
		p = p->next;
	}while (p != tail);
}

ST *insertnode (ST *tail, ST *s)
{
	//设数据域的值升序有序且无重复值，插入后新链表依然升序有序
	ST *p = tail;

	do {
		if (p->next->data >= s->data) {
			break;
		}
		else 
		      p = p->next;
	}while (p != tail);

	//判断是否待插结点是要成为新的尾结点，是则需要挪尾指针，否则直接执行插入算法
	if (p == tail && p->next->data < s->data) 
	      tail = s;

	//此时则只需要直接插入待插结点即可
	s->next = p->next;
	p->next = s;

	return tail;
}

ST *delnode (ST *tail, int delval)
{
	ST *p = tail, *delp;

	do {
		if (p->next->data == delval) 
		      break;
		else 
		      p = p->next;
	}while (p != tail);

	if (p == tail && p->next->data != delval) {
		//此时表明没有找到待删结点数据域值为delval的
		printf ("Not found!\n");
	}
	else {
		if (tail->next == tail) {
			free (tail);
			tail = NULL;
		}
		else {
			delp = p->next;
			p->next = delp->next;

			//判断是否待删结点是尾结点，如果是则需要挪尾指针，否则直接删
			if (delp == tail)
			      tail = p;

			free (delp);
		}
	}

	return tail;
}
		
int main(int argc, char *argv[])
{
	int a[N] = {10, 20, 30, 40, 50};
	ST *tail, *s;
	int delval;

	tail = creatlink (a);

	s = (ST *)malloc(sizeof (ST));
	printf ("please inout the insert_num:\n");
	scanf ("%d", &s->data);

	tail = insertnode (tail, s);

	printf ("please input trhe delnode:\n");
	scanf ("%d", &delval);

	tail = delnode (tail, delval);

	printlink (tail);

	return EXIT_SUCCESS;
}

