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

int	ft_exit(t_cmd *cmd, t_ms *minishell)
{
	t_token	*token;
	int		exit_status;

	exit_status = 0;	
	token = cmd->token->next;
	if (token)
	{
		ft_putstr_fd("exit\n", 1);
		if (!double_sign(token->value))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(token->value, 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit_status = 2;
		}
		else if (token->next)
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			minishell->status = 1;
			return (1);
		}
		else
			exit_status = ft_atoi(token->value);
	}
	else
		ft_putstr_fd("exit\n", 1);
	minishell->status = exit_status;
	free(minishell->pwd);
	free(minishell->current_prompt);
	free_env(minishell);
	free(minishell);
	exit(exit_status);
}

int	double_sign(char *str)
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
