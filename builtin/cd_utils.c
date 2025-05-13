/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:59:40 by skock             #+#    #+#             */
/*   Updated: 2025/05/10 15:46:49 by cmontaig         ###   ########.fr       */
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

	i = 0;
	while (path[i])
		i++;
	while (path[i] != '/')
	{
		path[i] = '\0';
		i--;
	}
	return (path);
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

void	go_back(t_ms *minishell)
{
	char	*last_folder;

	last_folder = NULL;
	last_folder = get_last_folder(getcwd(NULL, 0));
	chdir(last_folder);
	update_pwd(minishell);
}
