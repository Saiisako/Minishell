/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:43:47 by skock             #+#    #+#             */
/*   Updated: 2025/05/26 14:35:27 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_input_prompt(t_ms *minishell, char *input)
{
	if (!input)
		handle_null_input(minishell);
	if (!parsing_input(input, minishell))
	{
		print_error_message("", minishell);
		free_token_list(minishell->token);
		return (0);
	}
	add_history(input);
	if (setup_heredocs(minishell->cmd_list, minishell) != 0)
	{
		handle_heredoc_error(minishell, input);
		return (0);
	}
	return (1);
}

void	handle_heredoc_error(t_ms *ms, char *input)
{
	free_cmd_list(ms->cmd_list);
	ms->cmd_list = NULL;
	free(input);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	handle_null_input(t_ms *ms)
{
	write(1, "exit\n", 5);
	free_env(ms);
	free(ms->current_prompt);
	free(ms->pwd);
	free_array(ms->envp);
	free(ms);
	exit(0);
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

bool spagetti(char *input, t_ms *minishell)
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
		if (!handle_input_prompt(minishell, input))
		{
			free(input);
			continue ;
		}
		if (spagetti(input, minishell))
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
