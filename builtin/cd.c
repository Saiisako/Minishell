/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:08:59 by skock             #+#    #+#             */
/*   Updated: 2025/05/09 20:30:17 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	go_back(t_ms *minishell)
{
	char	*last_folder;

	last_folder = NULL;
	last_folder = get_last_folder(getcwd(NULL, 0));
	chdir(last_folder);
	update_pwd(minishell);
}

void	go_tilde(t_ms *minishell)
{
	chdir(get_user(minishell));
	update_pwd(minishell);
}

void	go_root(t_ms *minishell)
{
	chdir("/");
	update_pwd(minishell);
}

void	go_old(t_ms *minishell)
{
	chdir(get_oldpwd(minishell));
	update_pwd(minishell);
}

void	cd(t_cmd *cmd, t_ms *ms)
{
	t_token	*arg;

	if (!cmd || !cmd->token)
		return ;
		
	arg = cmd->token->next;
	if (!arg || (arg->value && !ft_strcmp(arg->value, "~")))
	{
		go_tilde(ms);
		return ;
	}
	if (!ft_strcmp(arg->value, "-"))
	{
		go_old(ms);
		return ;
	}
	if (!ft_strcmp(arg->value, ".."))
	{
		go_back(ms);
		return ;
	}
	if (!ft_strcmp(arg->value, "/"))
	{
		go_root(ms);
		return ;
	}
	if (chdir(arg->value) == 0)
	{
		update_pwd(ms);
		return ;
	}
	else
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_printf("%s: ", arg->value);
		ft_putstr_fd("No such file or directory\n", 2);
		ms->status = 1;
		exit(ms->status);
	}
}
