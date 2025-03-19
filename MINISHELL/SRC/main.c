#include "../INCLUDE/main.h"

int	main()
{
	t_gc_list *head = init_gc_list();
	printf("head : %p\n",head);
	char	*first;
	char	*second;
	
	do_alloc(head, 10);
	printf("first %p\n", head->next->data);

	do_alloc(head, 10);
	printf("second %p\n", head->next->data);

	printf("------------------------------------------------------------\n");
	print_list(head);
	gc_free(&head);
	printf("------------------------------------------------------------\n");
	if (head->next == NULL)
		printf("null");
}