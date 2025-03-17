#include "../INCLUDE/main.h"

int	main()
{
	t_gc_list *head = init_gc_list();
	char	*first;
	char	*second;
	
	first = do_alloc(head, 10);
	printf("test %p\n", first);
	gc_inc_ref_count(head,first);

	second = do_alloc(head, 10);
	printf("second %p\n", second);
	gc_inc_ref_count(head,second);

	printf("------------------------------------------------------------\n");
	print_list(head);
}