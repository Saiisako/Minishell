/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:24:48 by skock             #+#    #+#             */
/*   Updated: 2025/05/27 14:35:34 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_oldpwd(t_env *env_lst, char *old_pwd_value)
{
	t_env	*old;

	old = env_lst;
	while (old)
	{
		if (!ft_strcmp(old->key, "OLDPWD"))
		{
			free(old->value);
			old->value = old_pwd_value;
			return ;
		}
		old = old->next;
	}
	free(old_pwd_value);
}

int	old_pwdexist(t_env *env)
{
	t_env	*tmp;
	bool	found;

	found = false;
	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(env->key, "OLDPWD"))
		{
			found = true;
			break ;
		}
		tmp = tmp->next;
	}
	if (found)
		return (0);
	else
		return (1);
}

int	is_user_exist(t_env *env)
{
	t_env	*tmp;
	bool	found;

	found = false;
	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(env->key, "USER"))
		{
			found = true;
			break ;
		}
		tmp = tmp->next;
	}
	if (found)
		return (0);
	else
		return (1);
}
