/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:23:11 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/21 14:14:17 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	run_builtin_command(t_ms *minishell, t_cmd *cmd, char **args)
{
	int	result;

	result = 0;
	if (!ft_strcmp(args[0], "echo"))
		print_echo(cmd);
	else if (!ft_strcmp(args[0], "cd"))
		result = cd(cmd, minishell);
	else if (!ft_strcmp(args[0], "pwd"))
		print_pwd(minishell);
	else if (!ft_strcmp(args[0], "export"))
		result = ft_export(minishell, cmd);
	else if (!ft_strcmp(args[0], "unset"))
		ft_unset(minishell, cmd);
	else if (!ft_strcmp(args[0], "env"))
		print_env(minishell);
	else if (!ft_strcmp(args[0], "exit"))
		result = ft_exit(cmd, minishell, args);
	else
		return (minishell->status = 1, 1);
	minishell->status = result;
	return (result);
}

bool	is_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"));
}

int	execute_builtin(t_ms *minishell, char **args)
{
	t_cmd	cmd;
	t_token	first_token;
	int		result;

	if (init_cmd_and_token(&cmd, &first_token, args))
		return (1);
	result = run_builtin_command(minishell, &cmd, args);
	if (first_token.next)
		free_token_list(first_token.next);
	return (result);
}
