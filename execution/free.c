/* ************************************************************************** */
/*		                                                                    */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ChloeMontaigut <ChloeMontaigut@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 02:26:26 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/15 17:51:11 by ChloeMontai      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_token_list(t_token *token)
{
	t_token	*tmp;
	t_token	*next;

	tmp = token;
	while (tmp)
	{
		next = tmp->next;
		if (tmp->value)
			free(tmp->value);
		tmp->value = NULL;
		free(tmp);
		tmp = next;
	}
	token = NULL;
}

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*tmp;
	t_cmd	*next;

	tmp = cmd;
	while (tmp)
	{
		next = tmp->next;
		free_token_list(tmp->token);
		free(tmp->path);
		if (tmp->infile_fd != -2)
			close(tmp->infile_fd);
		if (tmp->outfile_fd != -2)
			close(tmp->outfile_fd);
		free(tmp);
		tmp = next;
	}
	cmd = NULL;
}

void	free_minishell(t_ms *minishell)
{
	if (!minishell)
		return ;
	if (minishell->cmd_list)
		free_cmd_list(minishell->cmd_list);
	if (minishell->token)
		free_token_list(minishell->token);
	if (minishell->expand)
		free_token_list(minishell->expand);
	if (minishell->env_lst)
		free_env(minishell);
	if (minishell->envp)
		free_array(minishell->envp);
	if(minishell->current_prompt)
		free(minishell->current_prompt);
	if(minishell->pwd)
		free(minishell->pwd);
	free(minishell);
}

void	free_array(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
	args = NULL;
}

void	free_env(t_ms *minishell)
{
	t_env	*tmp;
	t_env	*next_node;

	tmp = minishell->env_lst;
	while (tmp)
	{
		next_node = tmp->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = next_node;
	}
}

void	basic_free(t_ms *ms, char **args)
{
	free_array(ms->envp);
	free_env(ms);
	free_array(args);
	free_cmd_list(ms->cmd_list);
	free(ms->current_prompt);
	free(ms->pwd);
	free(ms->exec);
	free(ms);
}
