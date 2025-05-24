/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:11:10 by skock             #+#    #+#             */
/*   Updated: 2025/05/24 19:02:46 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handmade_merge_token(t_token **tmp)
{
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
			handmade_merge_token(&tmp);
		else
			tmp = tmp->next;
	}
}
