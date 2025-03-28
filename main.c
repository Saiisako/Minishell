/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:43:47 by skock             #+#    #+#             */
/*   Updated: 2025/03/28 19:32:02 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prompt(t_ms *minishell)
{
	char *input;

	printf("\033[H\033[J");
	while (1)
	{
		input = readline("minishell >");
		if (!parsing_input(input, minishell))
			print_error_message("error");
		if (input && *input)
			add_history(input);
		if (!ft_strncmp(input, "cd", 2))
			cd(minishell, input);
		if (!ft_strcmp(input, "env"))
			print_env(minishell);
		if (!ft_strcmp(input, "pwd"))
			print_pwd();
		free(input);
	}
}

int main(int ac, char **av, char **envp)
{
	t_ms *minishell;

	(void)av;
	minishell = malloc(sizeof(t_ms));
	minishell->envp = envp;
	minishell->is_next_space = false;
	if (ac == 1)
	{
		fill_env_cpy(minishell, envp);
		prompt(minishell);
		return (0);
	}
	return (1);
}

