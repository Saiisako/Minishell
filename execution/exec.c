/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:34:45 by skock             #+#    #+#             */
/*   Updated: 2025/05/19 16:35:46 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int	execute_pipeline(t_ms *ms, t_cmd *cmd)
// {
// 	int	pipe_fd[2]; 
// 	int	prev_p;
// 	int	last_pid;
// 	int	redir_ok;

// 	cmd = ms->cmd_list;
// 	prev_p = -1;
// 	last_pid = -1;
// 	while (cmd)
// 	{
// 		redir_ok = 1;
// 		if (process_redirections(cmd, ms))
// 		{
// 			ms->status = 1;
// 			redir_ok = 0;
// 		}
// 		if (setup_pipes(cmd, pipe_fd, &prev_p))
// 			return (1);
// 		if (redir_ok)
// 			last_pid = handle_command(ms, cmd, pipe_fd, &prev_p, &ms->status);
// 		else
// 			cleanup_pipes(cmd, pipe_fd, &prev_p);
// 		cmd = cmd->next;
// 	}
// 	return (wait_all_children(ms, last_pid, ms->status));
// }

int	execute_pipeline(t_ms *ms, t_cmd *cmd)
{
	int	pipe_fd[2]; 
	int	prev_p;
	int	last_pid;
	int	redir_ok;
	int	result;

	signal(SIGINT, handle_signal_exec);
	signal(SIGQUIT, handle_signal_exec);
	cmd = ms->cmd_list;
	prev_p = -1;
	last_pid = -1;
	while (cmd)
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
		redir_ok = 1;
		if (process_redirections(cmd, ms))
		{
			ms->status = 1;
			redir_ok = 0;
		}
		if (setup_pipes(cmd, pipe_fd, &prev_p))
			return (1);
		if (redir_ok)
			last_pid = handle_command(ms, cmd, pipe_fd, &prev_p, &ms->status);
		else
			last_pid = handle_redir_error(ms, cmd, pipe_fd, &prev_p);
		cmd = cmd->next;
	}
	result = wait_all_children(ms, last_pid, ms->status);
	// signal(SIGINT, handle_signal_prompt);
	// signal(SIGQUIT, SIG_IGN);
	return (result);
}

// try
int	handle_redir_error(t_ms *ms, t_cmd *cmd, int pipe_fd[2], int *prev_pipe)
{
	int	pid = fork();

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
			close(cmd->heredoc_fd); ///////
		ms->status = 1;
		exit (ms->status);
	}
	else if (pid > 0)
	{
		cmd->pid = pid;
		cleanup_pipes(cmd, pipe_fd, prev_pipe);
	}
	else
	{
		perror("minishell : fork");
		return (-1);
	}
	return (pid);
}

int	handle_command(t_ms *ms, t_cmd *cmd, int pipe_fd[2], int *prev_pipe, int *status)
{ 
	char	**args;
	int		pid;

	args = tokens_to_args(cmd->token);
	if (!args || !args[0])
	{
		pid = handle_empty_cmd(cmd, prev_pipe, pipe_fd, ms);
		free_array(args);
		return (pid);
	}
	if (!is_builtin(args[0]))
	{
		cmd->path = find_command_path(args[0], ms->env_lst);
		if (!cmd->path)
		{
			print_cmd_not_found(args[0]);
			free_array(args);
			cleanup_pipes(cmd, pipe_fd, prev_pipe);
			return (ms->status = 127, -1);
		}
	}
	pid = execute_cmd(ms, cmd, args, pipe_fd, *prev_pipe, status);
	update_fds(cmd, pipe_fd, prev_pipe);
	free_array(args);
	return (pid);
}

int	handle_empty_cmd(t_cmd *cmd, int *prev_pipe, int pipe_fd[2], t_ms *ms)
{
	int	child_pid;

	if (!cmd->is_redir)
	{
		ft_putstr_fd(": command not found", 2);
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

int	execute_cmd(t_ms *minishell, t_cmd *cmd, char **args, int *pipe_fd, int prev, int *status)
{
	if (!args || !args[0])
		return (-1);
	if (is_builtin(args[0]) && !cmd->next && prev == -1
		&& cmd->infile_fd == -2 && cmd->outfile_fd == -2)
	{
		*status = execute_builtin(minishell, args);
		free(cmd->path); //////
		cmd->path = NULL; ///////
		return (-1);
	}
	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		exec_redir(cmd, prev, pipe_fd, args, minishell);
		free(minishell->current_prompt);
		free(minishell->pwd);
		rl_clear_history();
		exit(EXIT_FAILURE); //ff
	}
	else if (cmd->pid < 0)
	{
		perror("minishell: fork");
		return (-1);
	}
	return (cmd->pid);
}

void	exec_redir(t_cmd *cmd, int prev, int *pipe_fd, char **args, t_ms *ms)
{
	if (cmd->heredoc_fd > 0)
		dup2(cmd->heredoc_fd, STDIN_FILENO);
	else if (cmd->infile_fd != -2)
		dup2(cmd->infile_fd, STDIN_FILENO);
	else if (prev != -1)
		dup2(prev, STDIN_FILENO);
	if (cmd->outfile_fd != -2)
		dup2(cmd->outfile_fd, STDOUT_FILENO);
	else if (cmd->next)
		dup2(pipe_fd[1], STDOUT_FILENO);
	if (prev != -1)
		close(prev);
	if (cmd->next)
	{
		close(pipe_fd[0]);
		if (cmd->outfile_fd != pipe_fd[1])
			close(pipe_fd[1]);
	}
	if (is_builtin(args[0]))
	{
		int ret = execute_builtin(ms, args);
		free_array(args); ////
		free_env(ms);
		free_token_list(ms->expand);
		free_cmd_list(ms->cmd_list);
		exit(ret);
	}
	if (!cmd->path) //
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		ms->status = 127;
		exit(ms->status);
	}
	if(is_directory(cmd->path))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		ms->status = 126;
		exit(ms->status);
	}

	if (access(cmd->path, F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		ms->status = 127;
		exit(ms->status);
	}
	if (access(cmd->path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		ms->status = 126;
		exit(ms->status);
	}

	execve(cmd->path, args, ms->envp);
	int errno_code = errno;
	free_array(args); // a voir
	free_minishell(ms); // a voir
	handle_error_exec(ms, args, errno_code);
}

void	handle_error_exec(t_ms *minishell, char **args, int	errno_code)
{
	if (errno_code == EACCES)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		minishell->status = 126;
		free_array(args); ////
		free_minishell(minishell);
		exit(minishell->status);
	}
	else if (errno_code == ENOENT)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		minishell->status = 127;
		free_array(args); ////
		free_minishell(minishell);
		exit(minishell->status);
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		perror(args[0]);
		free_array(args);
		free_minishell(minishell); ///
		exit(EXIT_FAILURE);
	}
}

// int	wait_all_children(t_ms *ms, int last_pid, int last_status)
// {
// 	t_cmd	*cmd;
// 	int		status;

// 	cmd = ms->cmd_list;
// 	while (cmd)
// 	{
// 		if (cmd->pid > 0)
// 		{
// 			waitpid(cmd->pid, &status, 0);
// 			if (cmd->pid == last_pid)
// 			{
// 				if (WIFEXITED(status))
// 					last_status = WEXITSTATUS(status);
// 				else if (WIFSIGNALED(status))
// 				{
// 					last_status = 128 + WTERMSIG(status);
// 					if (WTERMSIG(status) == SIGQUIT)
// 					{
// 						write(1, "Quit (core dumped)\n", 20);
// 						g_sig = 131;
// 					}
// 					else
// 					{
// 						write(1, "\n", 1);
// 						g_sig = 130;
// 					}
// 				}
// 				ms->status = last_status;
// 			}
// 		}
// 		cmd = cmd->next;
// 	}
// 	return (last_status);
// }