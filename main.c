/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:43:47 by skock             #+#    #+#             */
/*   Updated: 2025/05/31 17:33:55 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_sig = 0;

int	handle_input_prompt(t_ms *minishell, char *input)
{
	if (!input)
		handle_null_input(minishell);
	if (!parsing_input(input, minishell))
	{
		free_token_list(minishell->token);
		return (0);
	}
	add_history(input);
	if (setup_heredocs(minishell->cmd_list) != 0)
	{
		handle_heredoc_error(minishell, input);
		return (0);
	}
	return (1);
}

int	verif_parsing(t_ms *ms)
{
	if (ms->cmd_list->token->type == HEREDOC
		&& ms->cmd_list->token->next->type == WORD
		&& !ms->cmd_list->token->next->next)
	{
		if (ms->cmd_list)
			free_cmd_list(ms->cmd_list);
		return (1);
	}
	return (0);
}

bool	exec(char *input, t_ms *minishell)
{
	if (minishell->cmd_list)
	{
		if (verif_parsing(minishell))
		{
			free(input);
			return (true);
		}
		execute_pipeline(minishell, minishell->cmd_list);
		free_cmd_list(minishell->cmd_list);
		minishell->cmd_list = NULL;
	}
	return (false);
}

void	prompt(t_ms *minishell)
{
	char	*input;
	char	*full_prompt;

	while (1)
	{
		signal(SIGINT, handle_signal_prompt);
		signal(SIGQUIT, SIG_IGN);
		full_prompt = ft_strjoin(minishell->current_prompt, " > ");
		g_sig = 0;
		input = readline(full_prompt);
		free(full_prompt);
		if (g_sig == 1)
			minishell->status = 130;
		if (!handle_input_prompt(minishell, input))
		{
			free(input);
			continue ;
		}
		if (exec(input, minishell))
			continue ;
		free(input);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_ms	*minishell;

	minishell = NULL;
	(void)av;
	if (ac == 1)
	{
		setup_minishell(&minishell, envp);
		fill_env_cpy(minishell, envp);
		prompt(minishell);
		free_minishell(minishell);
		return (0);
	}
	return (1);
}
