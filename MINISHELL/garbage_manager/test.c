#include "garbage_collector.h"

// int main()
// {
// 	t_gc_list *gc_lst = init_gc_list();

// 	// char *single_ptr = (char*)do_alloc(gc_lst, 10, TYPE_SINGLE_PTR);
// 	// single_ptr = "Aaaa";
// 	char **double_ptr = (char**) do_alloc(gc_lst, sizeof(char*) *10);
// 	for(int i = 0; i < 5; i++)
// 	{
// 		double_ptr[i] = (char *)do_alloc(gc_lst, 10);
// 		if (double_ptr[i])
// 			strcpy(double_ptr[i], "HELLO");
// 		printf("%p, %s\n", double_ptr[i], double_ptr[i]);
// 	}
// 	t_gc_list *find = find_node(gc_lst, double_ptr[2]);

// 	t_gc_list *hey = find_node(gc_lst, double_ptr[4]);

// 	delete_node(&gc_lst, find);
// 	delete_node(&gc_lst, hey);

// 	print_list(gc_lst);

// 	all_free(&gc_lst);
// }


 cat -e | wc | ls

cat | wc

cat << eof

env << eof

cat ls wc 