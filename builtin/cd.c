/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:08:59 by skock             #+#    #+#             */
/*   Updated: 2025/03/19 11:22:16 by skock            ###   ########.fr       */
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
	printf("ehre\n");
	chdir(get_oldpwd(minishell));
	update_pwd(minishell);
}

void	cd(t_ms *minishell, char *input)
{
	char	**input_splited;

	input_splited = ft_split(input, ' ');
	if (!ft_strcmp(input_splited[0], "cd"))
	{
		go_tilde(minishell);
		return ;
	}
	if (!ft_strcmp(input_splited[1], ".."))
	{
		go_back(minishell);
		return ;
	}
	if (!ft_strcmp(input_splited[1], "-"))
	{
		go_old(minishell);
		return ;
	}
	if (!ft_strcmp(input_splited[1], "~"))
	{
		go_tilde(minishell);
		return ;
	}
	if (!ft_strcmp(input_splited[1], "/"))
	{
		go_root(minishell);
		return ;
	}
	if (!chdir(input_splited[1]))
	{
		update_pwd(minishell);
		return ;
	}
	if (!chdir(ft_strjoin_slash(getcwd(NULL, 0), input_splited[1])))
	{
		update_pwd(minishell);
		return ;
	}
}
