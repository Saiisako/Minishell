/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:15:22 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/27 19:32:26 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_redir_syntax_error(t_token *token, t_ms *ms)
{
	if (!token->next)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		ms->status = 1;
		return (1);
	}
	if (token->next->type != WORD && token->next->type != D_QUOTE
		&& token->next->type != S_QUOTE && token->next->type != EXPANDING)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		ms->status = 1;
		return (1);
	}
	return (0);
}

int	open_redirection(t_cmd *cmd, t_token *token, int type)
{
	int	fd;

	fd = -1;
	if (type == REDIR_IN)
		fd = open(token->next->value, O_RDONLY);
	else if (type == REDIR_OUT)
		fd = open(token->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == APPEND)
		fd = open(token->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (-1);
	if (type == REDIR_IN)
		cmd->infile_fd = fd;
	else
		cmd->outfile_fd = fd;
	cmd->is_redir = true;
	return (0);
}

int	handle_redirection_error(t_cmd *cmd, t_ms *ms, t_token *token)
{
	ft_putstr_fd("minishell: ", 2);
	perror(token->next->value);
	if (cmd->infile_fd != -2 && cmd->infile_fd != -1)
		close(cmd->infile_fd);
	ms->status = 1;
	return (1);
}

int	process_redirections(t_cmd *cmd, t_ms *ms)
{
	t_token	*token;
	int		type;

	token = cmd->token;
	while (token && token->type != PIPE)
	{
		type = token->type;
		if (type == REDIR_IN || type == REDIR_OUT || type == APPEND)
		{
			if (is_redir_syntax_error(token, ms))
				return (1);
			if (type == REDIR_IN && cmd->infile_fd != -2)
				close(cmd->infile_fd);
			if ((type == REDIR_OUT || type == APPEND)
				&& cmd->outfile_fd != -2)
				close(cmd->outfile_fd);
			if (open_redirection(cmd, token, type) == -1)
			{
				return (handle_redirection_error(cmd, ms, token));
			}
			token = token->next;
		}
		token = token->next;
	}
	return (0);
}

void	handle_redirections(t_cmd *cmd, int prev_pipe, int *pipe_fd)
{
	if (cmd->heredoc_fd > 0)
		dup2(cmd->heredoc_fd, STDIN_FILENO);
	else if (cmd->infile_fd != -2)
		dup2(cmd->infile_fd, STDIN_FILENO);
	else if (prev_pipe != -1)
		dup2(prev_pipe, STDIN_FILENO);
	if (cmd->outfile_fd != -2)
		dup2(cmd->outfile_fd, STDOUT_FILENO);
	else if (cmd->next)
		dup2(pipe_fd[1], STDOUT_FILENO);
	if (cmd->heredoc_fd > 0)
		close(cmd->heredoc_fd);
	else if (cmd->infile_fd != -2)
		close(cmd->infile_fd);
	if (cmd->outfile_fd != -2)
		close(cmd->outfile_fd);
	else if (cmd->next)
		close(pipe_fd[1]);
	if (cmd->next)
		close(pipe_fd[0]);
}
