/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:24:48 by skock             #+#    #+#             */
/*   Updated: 2025/05/28 15:31:38 by cmontaig         ###   ########.fr       */
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

void	print_error_cd(t_token *arg)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(arg->value, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}
