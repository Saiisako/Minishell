/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:59:40 by skock             #+#    #+#             */
/*   Updated: 2025/05/17 14:51:31 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_pwd(t_ms *minishell)
{
	t_env	*current;
	t_env	*old;
	char	*cwd;
	char	*old_pwd_value;

	old = minishell->env_lst;
	current = minishell->env_lst;
	while (current)
	{
		if (!ft_strcmp(current->key, "PWD"))
			break ;
		current = current->next;
	}
	old_pwd_value = ft_strdup(current->value);
	while (old)
	{
		if (!ft_strcmp(old->key, "OLDPWD"))
		{
			free(old->value);
			old->value = old_pwd_value;
			break ;
		}
		old = old->next;
	}
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		free(current->value);
		current->value = cwd;

		free(minishell->pwd);
		minishell->pwd = ft_strdup(cwd);
		
		free(minishell->current_prompt);
		minishell->current_prompt = ft_strdup(get_last_dir(cwd));
	}
	else
	{
		free(current->value);
		current->value = ft_strdup(minishell->pwd);

	}
	if (!old)
		free(old_pwd_value);
}

char	*get_last_folder(char *path)
{
	char	*result;
	char	*last_slash;
		
	if (!path)
		return NULL;
	result = ft_strdup(path);
	if (!result)
		return NULL;
	last_slash = ft_strrchr(result, '/');
	if (!last_slash || last_slash == result)
	{
		free(result);
		return ft_strdup("/");
	}
	*last_slash = '\0';
	if (result[0] == '\0')
	{
		free(result);
		return ft_strdup("/");
	}
	return result;
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
	char	*cwd;
	int		result;

	cwd = getcwd(NULL, 0);
	result = chdir("..");
	if (!cwd)
		ft_putstr_fd("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 2);
	if (result == 0)
		update_pwd(minishell);
	else
	{
		ft_putstr_fd("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 2);
		minishell->status = 1;
	}
	free(cwd);
}
