#include "../INCLUDE/main.h"

//pwd getcwd() get current working directory
char	*pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		//todo handling error case.
		return NULL;
	}
	return cwd;
}

// int main()
// {
// 	char *cwd = pwd();
// 	if (cwd)
// 	{
// 		printf("%p, %s\n", cwd, cwd);
// 		free(cwd);
// 		cwd = NULL;
// 		if (cwd == NULL)
// 			printf("\033[1;34mcwd is already free\033[0;34m\n");
// 	}
// }