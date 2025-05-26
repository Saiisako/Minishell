/* ************************************************************************** */
/*	                                                                        */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:54:49 by cmontaig          #+#    #+#             */
/*   Updated: 2025/04/12 13:13:17 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exit(t_cmd *cmd, t_ms *minishell, char **args)
{
	t_token		*token;
	int			exit_status;
	long long	numeric_value;
	int			res;

	exit_status = 0;
	token = cmd->token->next;
	ft_putstr_fd("exit\n", 1);
	if (token)
	{
		res = ft_exit_error(token, minishell, &numeric_value, args);
		if (res == 1)
			return (1);
		if (res == 2)
			exit_status = 2;
		else
			exit_status = (unsigned char)numeric_value;
	}
	exit_clean(minishell, args);
	exit(exit_status);
}

int	ft_exit_error(t_token *token, t_ms *ms,
	long long *numeric_value, char **args)
{
	if (!db_sign(token->value) || !ft_atoll(token->value, numeric_value))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(token->value, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		ms->status = 2;
		return (2);
		exit_clean(ms, args);
		exit(ms->status);
	}
	else if (token->next)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		ms->status = 1;
		return (1);
	}
	return (0);
}

int	db_sign(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	exit_clean(t_ms *ms, char **args)
{
	free(ms->pwd);
	free(ms->current_prompt);
	free_env(ms);
	free_array(ms->envp);
	free_array(args);
	free_cmd_list(ms->cmd_list);
	free(ms->exec);
	free(ms);
}
