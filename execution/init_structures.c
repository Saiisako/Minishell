/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:22:07 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/26 14:25:13 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_exec_struct(t_exec *exec)
{
	exec->prev_pipe = -1;
	exec->last_pid = -1;
	exec->pipe_fd[0] = -1;
	exec->pipe_fd[1] = -1;
	exec->status = 0;
	exec->redir_ok = -1;
	exec->args = NULL;
}

void	setup_cwd(t_ms **minishell)
{
	char	*cwd;
	char	*last;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		last = get_last_dir(cwd);
		(*minishell)->current_prompt = ft_strdup(last);
		(*minishell)->pwd = ft_strdup(cwd);
		free(cwd);
	}
	else
	{
		(*minishell)->current_prompt = ft_strdup("?");
		(*minishell)->pwd = ft_strdup("?");
	}
}

void	setup_minishell(t_ms **minishell, char **envp)
{
	*minishell = malloc(sizeof(t_ms));
	(*minishell)->status = 0;
	(*minishell)->envp = dup_envp(envp);
	(*minishell)->first_special = 42;
	(*minishell)->second_special = 42;
	(*minishell)->exec = NULL;
	(*minishell)->unexpected = false;
	(*minishell)->is_next_space = false;
	(*minishell)->go_cmd = true;
	(*minishell)->here_doc_expand = false;
	(*minishell)->is_pipe_error = false;
	setup_cwd(minishell);
}

int	create_token_chain(t_token *first_token, char **args)
{
	t_token	*current;
	t_token	*new_token;
	int		i;

	current = first_token;
	i = 1;
	while (args[i])
	{
		new_token = malloc(sizeof(t_token));
		if (!new_token)
			return (1);
		new_token->value = ft_strdup(args[i]);
		if (!new_token->value)
			return (1);
		new_token->is_next_space = true;
		new_token->type = WORD;
		new_token->index = i;
		new_token->next = NULL;
		current->next = new_token;
		current = new_token;
		i++;
	}
	return (0);
}

int	init_cmd_and_token(t_cmd *cmd, t_token *first_token, char **args)
{
	if (!args || !args[0])
		return (1);
	cmd->path = NULL;
	cmd->infile_fd = -2;
	cmd->outfile_fd = -2;
	cmd->heredoc_fd = -1;
	cmd->is_pipe = false;
	cmd->is_redir = false;
	cmd->pid = -1;
	cmd->next = NULL;
	first_token->value = args[0];
	first_token->is_next_space = true;
	first_token->type = WORD;
	first_token->index = 0;
	first_token->next = NULL;
	if (create_token_chain(first_token, args))
		return (1);
	cmd->token = first_token;
	return (0);
}
