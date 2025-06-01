/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:43:03 by cmontaig          #+#    #+#             */
/*   Updated: 2025/06/01 16:05:18 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_echo(t_cmd *cmd)
{
	t_token	*token;
	int		newline;
	bool	first;

	newline = 1;
	first = true;
	token = cmd->token->next;
	while (token && token->type == WORD && is_echo_n_flag(token->value))
	{
		newline = 0;
		token = token->next;
	}
	while (token && token->type == WORD)
	{
		if (!first)
			write(STDOUT_FILENO, " ", 1);
		write(STDOUT_FILENO, token->value, ft_strlen(token->value));
		first = false;
		token = token->next;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
}

int	is_echo_n_flag(const char *str)
{
	int	i;

	if (!str || str[0] != '-' || str[1] != 'n')
		return (0);
	i = 2;
	while (str[i] && str[i] == 'n')
		i++;
	return (str[i] == '\0');
}
