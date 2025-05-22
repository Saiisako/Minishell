/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 11:15:11 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/22 18:33:27 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	heredoc_child_free(t_ms *ms)
// {
// 	printf("here\n");
// 	if (ms->envp)
// 		free_array(ms->envp);
// 	if (ms->env_lst)
// 		free_env(ms);
// 	if(ms->current_prompt)
// 		free(ms->current_prompt);
// 	if(ms->pwd)
// 		free(ms->pwd);
// 	if (ms->expand)
// 		free_token_list(ms->expand);
// 	if (ms->cmd_list)
// 		free_cmd_list(ms->cmd_list);
// 	// if (ms->token)
// 	// 	free_token_list(ms->token);
// 	if(ms)
// 		free(ms);
// }

// int	setup_heredocs(t_cmd *cmd_list, t_ms *ms)
// {
// 	t_token	*token;
// 	int		fd;

// 	while (cmd_list)
// 	{
// 		token = cmd_list->token;
// 		while (token)
// 		{
// 			if (token->type == HEREDOC && token->next && token->next->value)
// 			{
// 				fd = create_heredoc(token->next->value, ms);
// 				if (fd == -1)
// 					return (1);
// 				cmd_list->heredoc_fd = fd;
// 				cmd_list->is_redir = true;
// 			}
// 			token = token->next;
// 		}
// 		cmd_list = cmd_list->next;
// 	}
// 	return (0);
// }

// void	heredoc_child(int write_fd, char *limiter, t_ms *minishell)
// {
// 	char	*line;
// 	char	*tmp_line[2];

// 	signal(SIGINT, SIG_DFL);
// 	signal(SIGQUIT, SIG_IGN);
// 	printf("PROUT\n");
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line || ft_strcmp(line, limiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		tmp_line[0] = line;
// 		tmp_line[1] = NULL;
// 		do_expand_heredoc(tmp_line, minishell);
// 		write(write_fd, tmp_line[0], ft_strlen(tmp_line[0]));
// 		write(write_fd, "\n", 1);
// 		free(line);
// 	}
// 	close(write_fd);
// 	heredoc_child_free(minishell);
// 	// (void)minishell;
// 	exit(0);
// }

// int	create_heredoc(char *limiter, t_ms *minishell)
// {

// 	int	fd[2];
// 	int	pid;
// 	int	status;

// 	if (pipe(fd) == -1)
// 		return (perror("pipe"), -1);
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		close(fd[0]);
// 		close(fd[1]);
// 		return (perror("fork"), -1);
// 	}
// 	if (pid == 0)
// 		heredoc_child(fd[1], limiter, minishell);
// 	close(fd[1]);
// 	printf("PIPI\n");
// 	waitpid(pid, &status, 0);
// 	if (WIFSIGNALED(status))
// 	{
// 		close(fd[0]);
// 		minishell->status = 128 + WTERMSIG(status);
// 		return (-1);
// 	}
// 	free_array(minishell->envp); ///
// 	free_env(minishell);
// 	free(minishell->pwd);
// 	printf("AAAAAAAH\n");
// 	return (fd[0]);
// }


/////

// int	create_heredoc(char *limiter, t_ms *minishell)
// {
// 	int		fd[2];
// 	char	*line;
// 	char	*tmp_line[2];

// 	if (pipe(fd) == -1)
// 		return (perror("pipe"), -1);
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line || ft_strcmp(line, limiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		tmp_line[0] = line;
// 		tmp_line[1] = NULL;
// 		do_expand_heredoc(tmp_line, minishell);
// 		write(fd[1], tmp_line[0], ft_strlen(tmp_line[0]));
// 		write(fd[1], "\n", 1);
// 		free(tmp_line[0]);
// 	}
// 	close(fd[1]);
// 	return (fd[0]);
// }

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

////

// void	signal_heredoc(int sig)
// {
// 	if (sig == SIGINT)
// 	{
// 		rl_on_new_line();
// 		close(0);
// 		g_sig = 99;
// 	}
// }

// int	here_doc_eof(void)
// {
// 	// ft_fprintf(2, "error: warning: here-document at line %d "
// 	// 	"delimited by end-of-line (wanted `%s')\n", all->num_line, limiter);
// 	g_sig = 0;
// 	return (1);
// }

// void free_write(int *pipe_fd, char *input)
// {
// 	write(pipe_fd[1], input, ft_strlen(input));
// 	write(pipe_fd[1], "\n", 1);
// 	free(input);
// }

// int	end_heredoc(char *input, int *pipe_fd, t_cmd *cmd)
// {
// 	free(input);
// 	close(pipe_fd[1]);
// 	if (cmd->infile_fd > 0)
// 		close(cmd->infile_fd);
// 	cmd->infile_fd = pipe_fd[0];
// 	return (0);
// }

// int	do_heredoc(t_ms *ms, t_cmd *cmd_list, char *limiter)
// {
// 	int		pipe_fd[2];
// 	char	*input;
// 	int		fd;

// 	fd = dup(0);
// 	signal(SIGINT, signal_heredoc);
// 	if (pipe(pipe_fd) == -1)
// 		return (perror("pipe"), -1);
// 	while (1)
// 	{
// 		input = readline("> ");
// 		if (!input && g_sig == 99)
// 		{
// 			dup2(fd, 0);
// 			g_sig = 130;
// 			signal(SIGINT, &signal_heredoc);
// 			break ;
// 		}
// 		if (!input && here_doc_eof())
// 			break ;
// 		if (ft_strcmp(limiter, input))
// 			break ;
// 		free_write(pipe_fd, input);
// 	}
// 	return (end_heredoc(input, pipe_fd, cmd_list));
// }