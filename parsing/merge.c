/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:11:10 by skock             #+#    #+#             */
/*   Updated: 2025/05/24 16:39:15 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	next(t_token *new_tk, t_token *tmp, t_ms *minishell, char *merged_value)
{
	new_tk = new_token(merged_value, minishell, 0);
	new_tk->is_next_space = tmp->next->is_next_space;
	new_tk->next = tmp->next->next;
	free(tmp->value);
	free(tmp->next->value);
	tmp->value = new_tk->value;
	tmp->is_next_space = new_tk->is_next_space;
	tmp->next = new_tk->next;
}

void	merge_token(t_ms *minishell)
{
	t_token	*tmp;
	t_token	*new_tk;
	char	*first_value;
	char	*second_value;
	char	*merged_value;

	tmp = minishell->token;
	new_tk = NULL;
	while (tmp && tmp->next)
	{
		if (tmp->is_next_space == true)
		{
			first_value = tmp->value;
			second_value = tmp->next->value;
			if (first_value[0] == '\0')
				merged_value = ft_strdup(second_value);
			else if (second_value[0] == '\0')
				merged_value = ft_strdup(first_value);
			else
				merged_value = ft_strjoin(first_value, second_value);
			next(new_tk, tmp, minishell, merged_value);
			return ;
		}
		tmp = tmp->next;
	}
}

void	handmade_merge_token(t_token **tmp, t_ms *minishell)
{
	(void)minishell;
	char	*new_line;
	t_token	*keep;

	if (!(*tmp)->next)
	{
		(*tmp)->is_next_space = false;
		return ;
	}
	if ((*tmp)->next->is_next_space == true)
		(*tmp)->is_next_space = true;
	else
		(*tmp)->is_next_space = false;
	new_line = ft_strjoin((*tmp)->value, (*tmp)->next->value);
	free((*tmp)->value);
	(*tmp)->value = new_line;
	keep = (*tmp)->next->next;
	free((*tmp)->next->value);
	free((*tmp)->next);
	(*tmp)->next = keep;
}

void	merge_inception(t_ms *minishell)
{
	t_token	*tmp;

	tmp = minishell->token;
	while (tmp)
	{
		if (tmp->is_next_space == true)
			handmade_merge_token(&tmp, minishell);
		else
			tmp = tmp->next;
	}
}
