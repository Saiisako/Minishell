/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:43:47 by skock             #+#    #+#             */
/*   Updated: 2025/03/10 14:14:53 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	copy_env(t_env *env, char **envp)
{
	int i;
	(void)env;

	i = 0;
	while (envp[i])
	{
		ft_strchr(envp[i], '=');
		i++;
	}
}

int	main(int ac, char **av, char **envp)
{
	(void)av;
	t_env	*env;
	char	*input;
	
	env = NULL;
	if (ac == 1)
	{
		copy_env(env, envp);
		printf("\033[H\033[J");
		while (1)
		{
			input = readline("minishell ➜ $");
			if (input && *input)
				add_history(input);
			printf("%s\n", input);
			free(input);
		}
		return (0);
	}
	return (1);
}