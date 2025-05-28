/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:59:40 by skock             #+#    #+#             */
/*   Updated: 2025/05/28 15:12:46 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_pwd(t_ms *minishell)
{
	t_env	*env;
	char	*cwd;
	char	*old_pwd_value;

	old_pwd_value = NULL;
	env = minishell->env_lst;
	while (env && ft_strcmp(env->key, "PWD"))
		env = env->next;
	if (!env)
		return ;
	old_pwd_value = ft_strdup(env->value);
	update_oldpwd(minishell->env_lst, old_pwd_value);
	cwd = getcwd(NULL, 0);
	free(env->value);
	if (cwd)
	{
		env->value = cwd;
		free(minishell->pwd);
		minishell->pwd = ft_strdup(cwd);
		free(minishell->current_prompt);
		minishell->current_prompt = ft_strdup(get_last_dir(cwd));
	}
	else
		env->value = ft_strdup(minishell->pwd);
}

char	*get_last_folder(char *path)
{
	char	*result;
	char	*last_slash;

	if (!path)
		return (NULL);
	result = ft_strdup(path);
	if (!result)
		return (NULL);
	last_slash = ft_strrchr(result, '/');
	if (!last_slash || last_slash == result)
	{
		free(result);
		return (ft_strdup("/"));
	}
	*last_slash = '\0';
	if (result[0] == '\0')
	{
		free(result);
		return (ft_strdup("/"));
	}
	return (result);
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

char	*search_home(t_ms *minishell)
{
	int		i;
	char	*home;
	int		count;

	home = minishell->const_pwd;
	i = 0;
	count = 0;
	while (home[i])
	{
		if (home[i] == '/')
		count++;
		if (count == 3)
		break ;
		i++;
	}
	return (ft_substr(minishell->const_pwd, 0, i));
}

char	*get_last_dir(char *path)
{
	char	*last_slash;

	if (!path)
		return ("?");
	last_slash = ft_strrchr(path, '/');
	if (!last_slash)
		return (path);
	return (last_slash + 1);
}
