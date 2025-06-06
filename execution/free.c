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
	if (ms->envp)
		free_array(ms->envp);
	if (ms->env_lst)
		free_env(ms);
	if (args)
		free_array(args);
	if (ms->cmd_list)
		free_cmd_list(ms->cmd_list);
	if (ms->current_prompt)
		free(ms->current_prompt);
	if (ms->pwd)
		free(ms->pwd);
	free(ms->exec);
	if (ms->const_pwd)
		free(ms->const_pwd);
	free(ms);
}
