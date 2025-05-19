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
	t_token		*token;
	int			exit_status;
	long long	numeric_value;

	exit_status = 0;	
	token = cmd->token->next;
	if (token)
	{
		ft_putstr_fd("exit\n", 1);
		if (!db_sign(token->value) || !ft_atoll(token->value, &numeric_value))
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
			exit_status = (unsigned char)numeric_value;
	}
	else
		ft_putstr_fd("exit\n", 1);
	exit_clean(minishell);
	minishell->status = exit_status;
	exit(exit_status);
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

int	ft_atoll(const char *str, long long *out)
{
	int			i;
	int			sign;
	long long	result;

	i = 0;
	sign = 1;
	result = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		int digit = str[i] - '0';

		if (sign == 1 && (result > (LLONG_MAX - digit) / 10))
			return (0);
		if (sign == -1 && (-result < (LLONG_MIN + digit) / 10))
			return (0);
		result = result * 10 + digit;
		i++;
	}
	*out = result * sign;
	return (1);
}

void	exit_clean(t_ms *ms)
{
	free(ms->pwd);
	free(ms->current_prompt);
	free_env(ms);
	free(ms);
}
