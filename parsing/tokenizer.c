/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:18:40 by skock             #+#    #+#             */
/*   Updated: 2025/05/25 16:15:44 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parsing_error(t_ms *minishell)
{
	t_token	*tmp;

	tmp = minishell->token;
	if (verif_first_token(minishell))
		return (0);
	while (tmp && tmp->next)
	{
		if (tmp->type == PIPE && tmp->next->type == PIPE)
			return (minishell->is_pipe_error = true, 0);
		else if (tmp->type != 1 && tmp->next->type != 1 && tmp->type != PIPE)
		{
			minishell->first_special = tmp->type;
			minishell->second_special = tmp->next->type;
			return (0);
		}
		tmp = tmp->next;
	}
	if (tmp && tmp->type != 1)
		return (0);
	tmp = minishell->token;
	if (tmp && tmp->type == PIPE)
		return (0);
	return (1);
}

void	ft_cmd(t_ms *minishell)
{
	fill_cmd_lst(minishell);
	cut_weird(minishell->cmd_list);
}

int	parsing_input(char *input, t_ms *minishell)
{
	int	i;

	minishell->expand = NULL;
	minishell->token = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && ft_iswhitespace(input[i]))
			i++;
		if (!input[i])
			break ;
		if (!process_token(input, &i, minishell))
			return (0);
		if (!input[i])
			break ;
		i++;
	}
	select_type(minishell);
	expand_token(minishell->token, minishell);
	clear_quote(minishell);
	merge_inception(minishell);
	if (!parsing_error(minishell))
		return (0);
	ft_cmd(minishell);
	return (1);
}
