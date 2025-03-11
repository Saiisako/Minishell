#include "../minishell.h"

void	print_pwd(void)
{
	char	path[256];

	if (getcwd(path, sizeof(path)) != NULL)
		printf("%s\n", path);
	else
		perror("getcwd");
}
