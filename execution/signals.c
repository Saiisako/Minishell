/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:29:28 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/28 16:48:42 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_sig = 0;

void	signal_pipe(int sig)
{
	if (sig == SIGPIPE)
		g_sig = 13;
}

int	wait_all_children(t_ms *ms, int last_pid, int last_status)
{
	t_cmd	*cmd;
	int		status;

	cmd = ms->cmd_list;
	while (cmd)
	{
		if (cmd->pid > 0)
		{
			waitpid(cmd->pid, &status, 0);
			if (cmd->pid == last_pid)
			{
				last_status = child_signal(status, last_status);
				ms->status = last_status;
			}
		}
		cmd = cmd->next;
	}
	return (last_status);
}

void	ft_signal(void)
{
	signal(SIGINT, handle_signal_exec);
	signal(SIGQUIT, handle_signal_exec);
	signal(SIGPIPE, signal_pipe);
}
