/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:29:28 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/22 18:33:14 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_sig = 0;

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
		else
		{
			write(1, "\n", 1);
			g_sig = 130;
		}
	}
	return (last_status);
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

void free_write(int *pipe_fd, char *input)
{
	write(pipe_fd[1], input, ft_strlen(input));
	write(pipe_fd[1], "\n", 1);
	free(input);
}

int	end_heredoc(char *input, int *pipe_fd, t_cmd *cmd)
{
	free(input);
	close(pipe_fd[1]);
	if (cmd->infile_fd > 0)
		close(cmd->infile_fd);
	cmd->infile_fd = pipe_fd[0];
	return (0);
}

int	do_heredoc(t_cmd *cmd_list, char *limiter)
{
	int		pipe_fd[2];
	char	*input;
	int		fd;

	fd = dup(0);
	signal(SIGINT, signal_heredoc);
	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), -1);
	while (1)
	{
		input = readline("> ");
		if (!input && g_sig == 99)
		{
			dup2(fd, 0);
			g_sig = 130;
			signal(SIGINT, &signal_heredoc);
			break ;
		}
		if (!input && here_doc_eof())
			break ;
		if (!ft_strcmp(limiter, input))
			break ;
		free_write(pipe_fd, input);
	}
	return (end_heredoc(input, pipe_fd, cmd_list));
}
