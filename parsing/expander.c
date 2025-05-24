/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:02:46 by skock             #+#    #+#             */
/*   Updated: 2025/05/24 18:27:00 by skock            ###   ########.fr       */
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
	{
		*tmp = (*tmp)->next;
	}
}

int	must_be_expand(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

void	handle_token(t_token **tmp, t_ms *minishell)
{
	t_token	*tmp2;

	tmp2 = (*tmp);
	if ((*tmp)->type == S_QUOTE)
		*tmp = (*tmp)->next;
	else if ((*tmp)->type == WORD && must_be_expand((*tmp)->value))
	{
		if (minishell->here_doc_expand == true)
		{
			minishell->here_doc_expand = false;
			*tmp = (*tmp)->next;
			return ;
		}
		handle_word_token(tmp, minishell);
	}
	else if ((*tmp)->type == D_QUOTE && must_be_expand((*tmp)->value))
	{
		(*tmp)->value = quote_rmv((*tmp)->value);
		(*tmp)->type = WORD;
		do_expand_dquote((*tmp)->value, minishell, (*tmp)->index);
		*tmp = (*tmp)->next;
	}
	else
		*tmp = (*tmp)->next;
}

void	is_heredoc_token(t_ms *ms)
{
	t_token	*tmp;

	tmp = ms->token;
	while (tmp->next)
	{
		if (tmp->type == HEREDOC && tmp->next->type == WORD)
			ms->here_doc_expand = true;
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
