/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 11:15:11 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/26 14:19:06 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	setup_heredocs(t_cmd *cmd_list, t_ms *minishell)
{
	t_token	*token;
	int		fd;

	while (cmd_list)
	{
		token = cmd_list->token;
		while (token)
		{
			if (token->type == HEREDOC && token->next && token->next->value)
			{
				fd = do_heredoc(minishell->cmd_list, token->next->value);
				if (fd == -1)
					return (1);
				cmd_list->heredoc_fd = fd;
				cmd_list->is_redir = true;
			}
			token = token->next;
		}
		cmd_list = cmd_list->next;
	}
	return (0);
}

void	free_write(int *pipe_fd, char *input)
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
