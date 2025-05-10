/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_update_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:50:24 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/10 16:55:30 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	env_add_new(t_env **env_lst, char *key, char *value, t_ms *ms)
{
	t_env	*current;

	current = malloc(sizeof(t_env));
	if (current == NULL)
		return (ms->status = 1, 1);
	current->key = ft_strdup(key);
	if (value != NULL)
		current->value = ft_strdup(value);
	else
		current->value = NULL;
	current->index = 0;
	current->next = *env_lst;
	*env_lst = current;
	return (0);
}

int	env_update_or_add(t_env **env_lst, char *key, char *value, t_ms *ms)
{
	t_env	*current;

	current = *env_lst;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			if (value != NULL)
				current->value = ft_strdup(value);
			else
				current->value = NULL;
			return (0);
		}
		current = current->next;
	}
	return (env_add_new(env_lst, key, value, ms));
}

void	update_envp(t_ms *ms)
{
	t_env	*env;
	int		count;
	int		i;

	env = ms->env_lst;
	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	ms->envp = malloc(sizeof(char *) * (count + 1));
	if (!ms->envp)
		return ;
	env = ms->env_lst;
	i = 0;
	while (env)
	{
		ms->envp[i] = concat_env_var(env->key, env->value);
		i++;
		env = env->next;
	}
	ms->envp[i] = NULL;
}
