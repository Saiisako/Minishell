/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_lst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:22:23 by skock             #+#    #+#             */
/*   Updated: 2025/05/26 11:39:06 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	is_heredoc_token(t_ms *ms)
{
	t_token	*tmp;

	tmp = ms->token;
	while (tmp && tmp->next)
	{
		if (tmp->type == HEREDOC && tmp->next->type == WORD)
			ms->here_doc_expand = true;
		tmp = tmp->next;
	}
}

t_token	*new_expand(char *str)
{
	t_token		*expand;

	expand = malloc(sizeof(t_token));
	expand->value = str;
	expand->next = NULL;
	expand->type = EXPANDING;
	expand->is_next_space = true;
	return (expand);
}

void	expand_add_back(t_token **lst, t_token *new)
{
	t_token	*last;

	if (!*lst)
		*lst = new;
	else
	{
		last = *lst;
		while (last->next != NULL)
			last = last->next;
		last->next = new;
	}
}

void	fill_expand_lst(t_ms *minishell, char *str)
{
	t_token	*expand;

	expand = new_expand(str);
	if (!expand)
		return ;
	expand_add_back(&minishell->expand, expand);
}
