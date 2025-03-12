/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:59:40 by skock             #+#    #+#             */
/*   Updated: 2025/03/12 17:00:39 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_pwd(t_ms *minishell)
{
	t_env	*current;
	t_env	*old;

	old = minishell->env_lst;
	current = minishell->env_lst;
	while (current)
	{
		if (!ft_strcmp(current->key, "PWD"))
			break ;
		current = current->next;
	}
	while (old)
	{
		if (!ft_strcmp(old->key, "OLDPWD"))
		{
			free(old->value);
			old->value = current->value;
		}
		old = old->next;
	}
	current->value = getcwd(NULL, 0);
}

char	*get_last_folder(char *path)
{
	int		i;
	int		j;
	char	*prev;

	i = 0;
	while (path[i])
		i++;
	j = i;
	while (path[i] != '/')
		i--;
	prev = malloc(sizeof(char) * (i + 1));
	if (!prev)
		exit(1);
	j = -1;
	while (++j < i)
		prev[j] = path[j];
	prev[j] = '\0';
	return (prev);
}

char	*get_user(t_ms *minishell)
{
	t_env	*tmp;

	tmp = minishell->env_lst;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "HOME"))
		{
			break ;
		}
		tmp = tmp->next;
	}
	return (tmp->value);
}

char	*get_oldpwd(t_ms *minishell)
{
	t_env	*tmp;

	tmp = minishell->env_lst;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "OLDPWD"))
			break ;
		tmp = tmp->next;
	}
	return (tmp->value);
}
