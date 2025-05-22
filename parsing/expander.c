/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:02:46 by skock             #+#    #+#             */
/*   Updated: 2025/05/22 19:29:09 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	delete_token(t_ms *minishell, char *value)
{
	t_token	*tmp;
	t_token	*prev;

	tmp = minishell->token;
	prev = NULL;
	while (tmp)
	{
		if (!ft_strcmp(tmp->value, value))
		{
			if (prev == NULL)
				minishell->token = tmp->next;
			else
				prev->next = tmp->next;
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void	handle_word_token(t_token **tmp, t_ms *minishell)
{
	t_token	*next;

	if (!ft_strcmp("$", (*tmp)->value))
	{
		*tmp = (*tmp)->next;
		return ;
	}
	do_expand((*tmp)->value, minishell, (*tmp)->index);
	if (!ft_strcmp((*tmp)->value, ""))
	{
		next = (*tmp)->next;
		delete_token(minishell, (*tmp)->value);
		*tmp = next;
	}
	else
		*tmp = (*tmp)->next;
}

void	handle_token(t_token **tmp, t_ms *minishell)
{
	t_token	*tmp2;

	tmp2 = (*tmp);
	if ((*tmp)->type == S_QUOTE)
		*tmp = (*tmp)->next;
	else if ((*tmp)->type == WORD)
	{
		if (minishell->caca == true)
		{
			minishell->caca = false;
			*tmp = (*tmp)->next;
			return ;
		}
		handle_word_token(tmp, minishell);
	}
	else if ((*tmp)->type == D_QUOTE)
	{
		do_expand((*tmp)->value, minishell, (*tmp)->index);
		*tmp = (*tmp)->next;
	}
	else
		*tmp = (*tmp)->next;
}

void	is_heredoc_token(t_ms *ms)
{
	t_token	*tmp;

	tmp = ms->token;
	while (tmp)
	{
		if (tmp->type == HEREDOC && tmp->next->type == WORD)
			ms->caca = true;
		tmp = tmp->next;
	}
}

void	expand_token(t_token *token, t_ms *minishell)
{
	t_token	*tmp;

	tmp = token;
	minishell->expand = NULL;
	is_heredoc_token(minishell);
	while (tmp)
		handle_token(&tmp, minishell);
}
