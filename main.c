/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:43:47 by skock             #+#    #+#             */
/*   Updated: 2025/04/02 18:54:00 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error_message(const char *msg)
{
	printf("%s\n", msg);
}

void	free_env(t_ms *minishell)
{
	t_env	*tmp;
	t_env	*next_node;

	tmp = minishell->env_lst;
	while (tmp)
	{
		next_node = tmp->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = next_node;
	}
}

void	prompt(t_ms *minishell)
{
	char	*input;

	while (1)
	{
		input = readline("minishell >");
		if (!input)
		{
			printf("CTRL + D\n");
			free_env(minishell);
			free(minishell);
			exit(0);
		}
		if (!parsing_input(input, minishell))
			print_error_message("error");
		if (input && *input)
			add_history(input);
		if (!ft_strcmp(input, "env"))
			print_env(minishell);
		if (!ft_strcmp(input, "pwd"))
			print_pwd();
		free(input);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_ms	*minishell;

	(void)av;
	if (ac == 1)
	{
		minishell = malloc(sizeof(t_ms));
		minishell->envp = envp;
		minishell->is_next_space = false;
		fill_env_cpy(minishell, envp);
		prompt(minishell);
		return (0);
	}
	return (1);
}
