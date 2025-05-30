/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:29:28 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/30 13:00:49 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_signal_prompt(int sig)
{
	if (sig == SIGINT)
	{
		g_sig = 1;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_signal_exec(int sig)
{
	if (sig == SIGINT)
		g_sig = 130;
	else if (sig == SIGQUIT)
		g_sig = 131;
}

int	child_signal(int status, int last_status)
{
	if (WIFEXITED(status))
	{
		last_status = WEXITSTATUS(status);
		return (last_status);
	}
	else if (WIFSIGNALED(status))
	{
		last_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
		{
			write(1, "Quit (core dumped)\n", 20);
			g_sig = 131;
		}
		else if (WTERMSIG(status) == SIGINT)
		{
			write(1, "\n", 1);
			g_sig = 130;
		}
	}
	return (last_status);
}

void	signal_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		close(0);
		g_sig = 99;
	}
}

int	here_doc_eof(void)
{
	g_sig = 0;
	return (1);
}
