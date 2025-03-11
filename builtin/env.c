
#include "../minishell.h"

void	print_env(t_ms *minishell)
{
	t_env	*tmp;

	tmp = minishell->env_lst;
	while (tmp)
	{
		if (tmp->value)
		{
			printf("%s", tmp->key);
			printf("=");
			printf("%s\n", tmp->value);
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
	return ;
}
