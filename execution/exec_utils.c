/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:14:03 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/30 12:51:22 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_cmd_not_found(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found", 2);
	write(2, "\n", 1);
}

void	cleanup_pipes(t_cmd *cmd, int pipe_fd[2], int *prev_pipe)
{
	if (*prev_pipe != -1)
	{
		close(*prev_pipe);
		*prev_pipe = -1;
	}
	if (cmd->next)
	{
		if (pipe_fd[1] != -1)
			close(pipe_fd[1]);
		*prev_pipe = pipe_fd[0];
	}
	else
	{
		if (pipe_fd[0] != -1)
			close(pipe_fd[0]);
	}
}

int	setup_pipes(t_cmd *cmd, int *pipe_fd, int *prev_pipe)
{
	if (cmd->next && pipe(pipe_fd) == -1)
	{
		if (*prev_pipe != -1)
			close(*prev_pipe);
		return (perror("pipe"), 1);
	}
	if (*prev_pipe != -1)
	{
		if (cmd->infile_fd == -2)
			cmd->infile_fd = *prev_pipe;
		else
			close(*prev_pipe);
	}
	if (cmd->next && cmd->outfile_fd == -2)
		cmd->outfile_fd = pipe_fd[1];
	else if (cmd->next)
		close(pipe_fd[1]);
	return (0);
}

void	update_fds(t_cmd *cmd, int *pipe_fd, int *prev_pipe)
{
	if (cmd->infile_fd != -2 && cmd->infile_fd != *prev_pipe)
		close(cmd->infile_fd);
	if (cmd->outfile_fd != -2 && (!cmd->next || cmd->outfile_fd != pipe_fd[1]))
		close(cmd->outfile_fd);
	if (*prev_pipe != -1)
		close(*prev_pipe);
	if (cmd->heredoc_fd > 0)
		close(cmd->heredoc_fd);
	if (cmd->next && pipe_fd[1] != -1)
	{
		close(pipe_fd[1]);
		*prev_pipe = pipe_fd[0];
	}
	else
		*prev_pipe = -1;
}

int	is_directory(const char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		return (1);
	}
	return (0);
}
