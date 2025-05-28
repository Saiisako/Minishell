/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_errors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:49:23 by skock             #+#    #+#             */
/*   Updated: 2025/05/28 12:11:27 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	what_type(t_type type)
{
	if (type == REDIR_IN || type == REDIR_OUT || type == APPEND || type == PIPE
		|| type == HEREDOC)
		return (1);
	else
		return (0);
}

int	verif_first_token(t_ms *minishell)
{
	t_token	*tmp;

	tmp = minishell->token;
	if (tmp && !tmp->next && tmp->type == PIPE)
	{
		printf("bash: syntax error near unexpected token '|'\n");
		return (1);
	}
	tmp = minishell->token;
	if (tmp && !tmp->next && (tmp->type != PIPE && tmp->type != WORD))
	{
		printf("bash: syntax error near unexpected token 'newline'\n");
		return (1);
	}
	tmp = minishell->token;
	if (tmp && tmp->type == PIPE)
	{
		printf("bash: syntax error near unexpected token '|'\n");
		return (1);
	}
	return (0);
}

int	verif_several_pipes(t_ms *minishell)
{
	t_token	*tmp;

	tmp = minishell->token;
	while (tmp)
	{
		if (tmp && tmp->next && tmp->type == PIPE && tmp->next->type == PIPE)
		{
			printf("bash: syntax error near unexpected token '|'\n");
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	verif_consecutives_tokens(t_ms *minishell)
{
	t_token	*tmp;

	tmp = minishell->token;
	while (tmp)
	{
		if ((tmp && tmp->next) && (what_type(tmp->type)
				&& what_type(tmp->next->type)))
		{
			if (tmp->type == PIPE && (tmp->next->type == APPEND
					|| tmp->next->type == REDIR_IN
					|| tmp->next->type == REDIR_OUT
					|| tmp->next->type == HEREDOC))
				return (0);
			printf("bash: syntax error near unexpected token '%s'\n",
				tmp->value);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	verif_last_token(t_ms *minishell)
{
	t_token	*tmp;

	tmp = minishell->token;
	while (tmp)
		tmp = tmp->next;
	if (tmp && tmp->type == PIPE)
	{
		printf("bash: syntax error near unexpected token '|'\n");
		return (1);
	}
	else if (tmp && tmp->type != WORD && tmp->type != PIPE)
	{
		printf("bash: syntax error near unexpected token 'newline'\n");
		return (1);
	}
	return (0);
}
