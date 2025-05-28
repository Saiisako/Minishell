/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:34:45 by skock             #+#    #+#             */
/*   Updated: 2025/05/28 15:34:33 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_pipeline(t_ms *ms, t_cmd *cmd)
{
	int		result;
	t_exec	*exec;

	signal(SIGINT, handle_signal_exec);
	signal(SIGQUIT, handle_signal_exec);
	signal(SIGPIPE, SIG_IGN);
	exec = malloc(sizeof(t_exec));
	if (!exec)
		return (1);
	cmd = ms->cmd_list;
	ms->exec = exec;
	init_exec_struct(exec);
	while (cmd)
	{
		if (run_cmd(ms, cmd, exec))
		{
			free(exec);
			return (1);
		}
		cmd = cmd->next;
	}
	result = wait_all_children(ms, exec->last_pid, ms->status);
	signal(SIGPIPE, SIG_DFL);
	free(exec);
	return (result);
}

int	run_cmd(t_ms *ms, t_cmd *cmd, t_exec *exec)
{
	int	redir_ok;

	redir_ok = 1;
	if (process_redirections(cmd, ms))
	{
		ms->status = 1;
		redir_ok = 0;
	}
	if (setup_pipes(cmd, exec->pipe_fd, &exec->prev_pipe))
		return (1);
	if (redir_ok)
		exec->last_pid = handle_command(ms, cmd, exec);
	else
		exec->last_pid = redir_error(ms, cmd, exec->pipe_fd, &exec->prev_pipe);
	return (0);
}

int	handle_command(t_ms *ms, t_cmd *cmd, t_exec *exec)
{
	char	**args;
	int		pid;

	args = tokens_to_args(cmd->token);
	if (!args || !args[0])
	{
		pid = handle_empty_cmd(cmd, &exec->prev_pipe, exec->pipe_fd, ms);
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
			cleanup_pipes(cmd, exec->pipe_fd, &exec->prev_pipe);
			return (ms->status = 127, -1);
		}
	}
	pid = execute_cmd(ms, cmd, args, exec);
	update_fds(cmd, exec->pipe_fd, &exec->prev_pipe);
	free_array(args);
	return (pid);
}

int	execute_cmd(t_ms *minishell, t_cmd *cmd, char **args, t_exec *exec)
{
	if (!args || !args[0])
		return (-1);
	if (is_builtin(args[0]) && !cmd->next && exec->prev_pipe == -1
		&& cmd->infile_fd == -2 && cmd->outfile_fd == -2)
	{
		exec->status = execute_builtin(minishell, args);
		free(cmd->path);
		cmd->path = NULL;
		return (-1);
	}
	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		exec_redir(cmd, exec, args, minishell);
		free(minishell->current_prompt);
		free(minishell->pwd);
		free(minishell->const_pwd);
		rl_clear_history();
		exit(EXIT_FAILURE);
	}
	if (cmd->pid < 0)
		return (perror("minishell: fork"), -1);
	return (cmd->pid);
}

void	exec_redir(t_cmd *cmd, t_exec *exec, char **args, t_ms *ms)
{
	int	ret;
	int	errno_code;

	handle_redirections(cmd, exec->prev_pipe, exec->pipe_fd);
	if (is_builtin(args[0]))
	{
		ret = execute_builtin(ms, args);
		free_token_list(ms->expand);
		basic_free(ms, args);
		exit(ret);
	}
	check_exec_errors(cmd, args, ms);
	execve(cmd->path, args, ms->envp);
	errno_code = errno;
	free_array(args);
	free_minishell(ms);
	handle_error_exec(ms, args, errno_code);
}
