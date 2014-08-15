//creat a empty link
struct line *creatlink ()
{
	struct line 	*head, *p;

	head = p = ( struct line * ) malloc ( sizeof ( struct line ) );

	p->next = NULL;

	return head;
}
