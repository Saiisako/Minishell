/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:08:59 by skock             #+#    #+#             */
/*   Updated: 2025/05/28 15:39:11 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	special_cd(t_token *arg, t_ms *ms)
{
	if (!arg || (arg->value && !ft_strcmp(arg->value, "~")))
	{
		chdir(get_user(ms));
		update_pwd(ms);
	}
	else if (!ft_strcmp(arg->value, ".."))
		go_back(ms);
	else if (!ft_strcmp(arg->value, "/"))
	{
		chdir("/");
		update_pwd(ms);
	}
	else
		ms->status = -1;
}

void	special_cd_env_i(t_token *arg, t_ms *ms)
{
	char	*home;

	if (!arg || (arg->value && !ft_strcmp(arg->value, "~")))
	{
		home = search_home(ms);
		chdir(home);
		free(home);
		update_pwd(ms);
	}
	else if (!ft_strcmp(arg->value, ".."))
		go_back(ms);
	else if (!ft_strcmp(arg->value, "/"))
	{
		chdir("/");
		update_pwd(ms);
	}
	else
		ms->status = -1;
}

int	return_status(t_ms *ms, t_token *arg)
{
	if (ms->status == -1)
	{
		if (chdir(arg->value) == 0)
		{
			update_pwd(ms);
			ms->status = 0;
		}
		else
		{
			print_error_cd(arg);
			return (ms->status = 1);
		}
	}
	return (ms->status);
}

int	cd(t_cmd *cmd, t_ms *ms)
{
	t_token	*arg;

	if (!cmd || !cmd->token)
		return (ms->status = 1);
	arg = cmd->token->next;
	if (arg && arg->next)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (ms->status = 1);
	}
	if (ms->env_i == true)
	{
		special_cd_env_i(arg, ms);
		ms->status = return_status(ms, arg);
	}
	else
	{
		special_cd(arg, ms);
		ms->status = return_status(ms, arg);
	}
	return (ms->status);
}

void	go_back(t_ms *minishell)
{
	char	*cwd;
	int		result;

	cwd = getcwd(NULL, 0);
	result = chdir("..");
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory: getcwd: ", 2);
		ft_putstr_fd("cannot access parent directories: No such file ", 2);
		ft_putstr_fd("or directory\n", 2);
	}
	if (result == 0)
		update_pwd(minishell);
	else
	{
		ft_putstr_fd("cd: error retrieving current directory: getcwd: ", 2);
		ft_putstr_fd("cannot access parent directories: No such file ", 2);
		ft_putstr_fd("or directory\n", 2);
		minishell->status = 1;
	}
	free(cwd);
}
