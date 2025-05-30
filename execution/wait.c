/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:13:55 by skock             #+#    #+#             */
/*   Updated: 2025/05/30 13:00:53 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
