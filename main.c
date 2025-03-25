/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:43:47 by skock             #+#    #+#             */
/*   Updated: 2025/03/25 12:40:44 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error_message(const char *msg)
{
	printf("%s\n", msg);
}

int	is_special_char(char cur, char next)
{
	if (next == cur && (cur == '>' || cur == '<'))
	{
		if (cur == '>')
			return (APPEND);
		if (cur == '<')
			return (HEREDOC);
	}
	return (2);
	if (cur == '|')
		return (PIPE);
	if (cur == '>')
		return (REDIR_OUT);
	if (cur == '<')
		return (REDIR_IN);
	return (0);
}

void prompt(t_ms *minishell)
{
	char *input;

	printf("\033[H\033[J");
	while (1)
	{
		get_input_prompt(minishell);
		input = readline("minishell >");
		if (input && *input)
			add_history(input);
		if (!ft_strncmp(input, "cd", 2))
			cd(minishell, input);
		if (!ft_strcmp(input, "env"))
			print_env(minishell);
		if (!ft_strcmp(input, "pwd"))
			print_pwd();
		if (!parsing_input(input, minishell))
			print_error_message("error");
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

