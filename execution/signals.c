/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:29:28 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/26 14:20:26 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_sig = 0;

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
