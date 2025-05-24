/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:19:36 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/24 18:19:14 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_error_exec(t_ms *minishell, char **args, int errno_code)
{
	if (errno_code == EACCES)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		minishell->status = 126;
	}
	else if (errno_code == ENOENT)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		minishell->status = 127;
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		perror(args[0]);
		free_array(args);
		free_minishell(minishell);
		exit(EXIT_FAILURE);
	}
	free_array(args);
	free_minishell(minishell);
	exit(minishell->status);
}

void	errors_prompt(int param, char **args, t_cmd *cmd)
{
	if (param == 0)
	{
		ft_putstr_fd("minishell:", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	else if (param == 1)
	{
		ft_putstr_fd("minishell:", 2);
		ft_putstr_fd(cmd->path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
	}
	else if (param == 2)
	{
		ft_putstr_fd("minishell:", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
	else
	{
		ft_putstr_fd("minishell:", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
	}
}

void	check_exec_errors(t_cmd *cmd, char **args, t_ms *ms)
{
	if (!cmd->path)
	{
		errors_prompt(0, args, cmd);
		exit(ms->status = 127);
	}
	if (is_directory(cmd->path))
	{
		errors_prompt(1, args, cmd);
		exit(ms->status = 126);
	}
	if (access(cmd->path, F_OK) != 0)
	{
		errors_prompt(2, args, cmd);
		exit(ms->status = 127);
	}
	if (access(cmd->path, X_OK) != 0)
	{
		errors_prompt(3, args, cmd);
		exit(ms->status = 126);
	}
}

int	redir_error(t_ms *ms, t_cmd *cmd, int pipe_fd[2], int *prev_pipe)
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		if (*prev_pipe != -1)
			close(*prev_pipe);
		if (cmd->next)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
		}
		if (cmd->heredoc_fd > 0)
			close(cmd->heredoc_fd);
		ms->status = 1;
		exit (ms->status);
	}
	if (pid < 0)
		return (perror("minishell : fork"), -1);
	cmd->pid = pid;
	cleanup_pipes(cmd, pipe_fd, prev_pipe);
	return (pid);
}

void	print_error_message(const char *msg, t_ms *minishell)
{
	char	*special;

	if (minishell->unexpected)
	{
		printf("bash: syntax error near unexpected token 'newline'\n");
		minishell->unexpected = false;
		return ;
	}
	if (minishell->first_special != 69 && minishell->second_special != 69)
	{
		if (minishell->second_special == HEREDOC)
			special = ft_strdup("<<");
		if (minishell->second_special == APPEND)
			special = ft_strdup(">>");
		if (minishell->second_special == REDIR_IN)
			special = ft_strdup("<");
		if (minishell->second_special == REDIR_OUT)
			special = ft_strdup(">");
		if (minishell->second_special == PIPE)
			special = ft_strdup("|");
		printf("bash: syntax error near unexpected token '%s'\n", special);
		free(special);
	}
	else
		printf("%s\n", msg);
}

int	handle_empty_cmd(t_cmd *cmd, int *prev_pipe, int pipe_fd[2], t_ms *ms)
{
	int	child_pid;

	if (!cmd->is_redir)
	{
		ft_putstr_fd(": command not found\n", 2);
		cleanup_pipes(cmd, pipe_fd, prev_pipe);
		return (ms->status = 127, -1);
	}
	child_pid = fork();
	if (child_pid == 0)
	{
		handle_redirections(cmd, *prev_pipe, pipe_fd);
		exit(EXIT_SUCCESS);
	}
	else if (child_pid > 0)
		cmd->pid = child_pid;
	cleanup_pipes(cmd, pipe_fd, prev_pipe);
	return (child_pid);
}
