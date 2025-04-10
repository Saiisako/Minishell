/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:43:03 by cmontaig          #+#    #+#             */
/*   Updated: 2025/04/10 16:44:44 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_echo(t_cmd *cmd)
{
	t_token	*token;
	int		newline = 1;
	bool	first = true;

	token = cmd->token->next;
	if (token && token->type == WORD && ft_strcmp(token->value, "-n") == 0)
	{
		newline = 0;
		token = token->next;
	}

	while (token && token->type == WORD)
	{
		if (!first)
			ft_printf(" ");
		ft_printf("%s", token->value);
		first = false;
		token = token->next;
	}
	if (newline)
		ft_printf("\n");
}

// $USER